/*  bezier_editor.cpp - edit beziers on canvas
 *  Copyright (C) 2017-2018 caryoscelus
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fmt/format.h>

#include <QGraphicsItem>
#include <QDebug>
#include <QMouseEvent>

#include <core/action_stack.h>
#include <core/actions/change_value.h>
#include <core/nullptr.h>

#include <util/strings.h>
#include <util/pen.h>
#include <util/qt_path.h>
#include <util/geom.h>
#include <widgets/canvas.h>
#include "point_item.h"
#include "bezier_editor.h"

using namespace fmt::literals;

namespace rainynite::studio {

BezierEditor::BezierEditor() {
    set_curve_pen(pens::cosmetic_dash());
}

BezierEditor::~BezierEditor() {
    uninit();
}

void BezierEditor::set_display_tags(bool display_tags_) {
    if (display_tags != display_tags_) {
        display_tags = display_tags_;
        if (display_tags) {
            add_tags();
        } else {
            remove_tags();
        }
    }
}

void BezierEditor::set_curve_pen(QPen const& pen) {
    curve_pen = pen;
    if (curve_item)
        curve_item->setPen(curve_pen);
}

struct BezierEditor::EventFilter : public QObject {
    EventFilter(BezierEditor* parent_) :
        parent(parent_)
    {}

    bool eventFilter(QObject* /*target*/, QEvent* event) override {
        return parent->canvas_event(event);
    }

    BezierEditor* parent;
};

void BezierEditor::setup_canvas() {
    uninit();

    if (auto canvas = get_canvas()) {
        if (get_bezier_node()) {
            init();
        }
        event_filter = make_unique<EventFilter>(this);
        canvas->installEventFilter(event_filter.get());
    }
}

bool BezierEditor::canvas_event(QEvent* event) {
    if (!appending)
        return false;
    qDebug() << event->type();
    if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
        auto pos = convert_pos(mouse_event->pos());
        switch (mouse_event->type()) {
            case QEvent::MouseButtonPress: {
                qDebug() << "hello";
                if (mouse_event->button() == Qt::LeftButton) {
                    drawing = true;
                    if (auto node = get_bezier_node()) {
                        if (node->is_const()) {
                            // TODO: support editing non-const
                            auto path = node->mod();
                            path.emplace_back(pos);
                            auto action_stack = get_context()->action_stack();
                            action_stack->emplace<core::actions::ChangeValue>(
                                node,
                                path
                            );
                            add_knot_editor(path.size()-1);
                            reset_curve(path);
                        }
                    }
//                     return true;

                    return false;
                }
            }
            case QEvent::MouseMove: {
                if (drawing) {
                    return true;
                }
            }
            case QEvent::MouseButtonRelease: {
                if (drawing) {
                    //
                    return true;
                }
            }
            default:
                break;
        }
    }
    return false;
}

Geom::Point BezierEditor::convert_pos(QPoint const& src) const {
    // TODO: transforms
    return util::point(get_canvas()->mapToScene(src));
}

void BezierEditor::node_update() {
    redraw();
}

void BezierEditor::time_changed(core::Time) {
    redraw();
}

void BezierEditor::redraw() {
    if (get_scene() && get_bezier_node()) {
        auto path = get_path();
        // NOTE: this is to avoid full redraw while editing
        // TODO: make sure it gets properly updated on non-editing changes
        if ((ptrdiff_t)path.size() == old_size) {
            curve_item->setPath(util::path_to_qt(path));
        } else {
            uninit();
            init();
        }
    }
}

void BezierEditor::init() {
    auto bezier_node = no_null(get_bezier_node());

    auto path = get_path();

    old_size = path.size();

    reset_curve(path);

    if (bezier_node->can_set()) {
        for (ptrdiff_t i = 0; i < old_size; ++i) {
            add_knot_editor(i);
        }
    }

    if (display_tags) {
        add_tags();
    }
}

void BezierEditor::add_tags() {
    auto path = get_path();
    for (auto const& knot : path.knots) {
        if (!knot.uid.empty()) {
            auto e = no_null(get_scene())->addText(util::str(knot.uid));
            e->setX(knot.pos.x());
            e->setY(knot.pos.y());
            tag_items.emplace_back(e);
        }
    }
}

QGraphicsItem* BezierEditor::add_point_editor(size_t i, Geom::Point Geom::Knot::* pref, QGraphicsItem* parent /*= nullptr*/) {
    auto bezier_node = no_null(get_bezier_node());
    auto e = new PointItem(
        [this, i, bezier_node, pref](double x, double y) {
            auto path = bezier_node->mod();
            auto& point = path.knots[i].*pref;
            point.x() = x;
            point.y() = y;

            auto action_stack = get_context()->action_stack();
            action_stack->emplace<core::actions::ChangeValue>(
                bezier_node,
                path
            );
            action_stack->close();
        }
    );
    if (parent) {
        e->setParentItem(parent);
        e->set_color({0xff, 0xff, 0x88});
    } else {
        no_null(get_scene())->addItem(e);
        e->set_color({0xff, 0x66, 0x66});
    }
    auto point = bezier_node->mod().knots[i].*pref;
    e->set_pos(point.x(), point.y());
    e->set_readonly(false);
    return e;
}

void BezierEditor::add_knot_editor(size_t i) {
    auto pos = add_point_editor(i, &Geom::Knot::pos);
    add_point_editor(i, &Geom::Knot::tg1, pos);
    add_point_editor(i, &Geom::Knot::tg2, pos);
    knot_items.emplace_back(pos);
}

void BezierEditor::reset_curve(Geom::BezierKnots const& path) {
    auto scene = no_null(get_scene());
    curve_item.reset(scene->addPath(util::path_to_qt(path)));
    curve_item->setPen(curve_pen);
}

void BezierEditor::uninit() {
    old_size = -1;
    if (auto scene = get_scene()) {
        for (auto const& e : knot_items)
            scene->removeItem(e.get());
    }
    knot_items.clear();
    curve_item.reset();
    remove_tags();
}

void BezierEditor::remove_tags() {
    if (auto scene = get_scene()) {
        for (auto const& e : tag_items)
            scene->removeItem(e.get());
    }
    tag_items.clear();
}

shared_ptr<core::BaseValue<Geom::BezierKnots>> BezierEditor::get_bezier_node() const {
    return get_node_as<Geom::BezierKnots>();
}

Geom::BezierKnots BezierEditor::get_path() const {
    return no_null(get_bezier_node())->value(get_core_context());
}

} // namespace rainynite::studio
