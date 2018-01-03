/*  bezier_editor.cpp - edit beziers on canvas
 *  Copyright (C) 2017 caryoscelus
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
    init();
}

BezierEditor::~BezierEditor() {
    uninit();
}

void BezierEditor::set_display_tags(bool display_tags_) {
    if (display_tags != display_tags_) {
        display_tags = display_tags_;
        uninit();
        init();
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
    init();

    if (auto canvas = get_canvas()) {
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
    if (auto scene = get_scene()) {
        if (auto bezier_node = get_bezier_node()) {
            bool readonly = !bezier_node->can_set();

            Geom::BezierKnots path;
            try {
                path = bezier_node->value(get_core_context());
            } catch (std::exception const& ex) {
                qDebug() << util::str("Uncaught exception while getting path: {}"_format(ex.what()));
                return;
            }

            old_size = path.size();

            curve_item.reset(scene->addPath(util::path_to_qt(path)));
            curve_item->setPen(curve_pen);

            auto add_point_editor = [
                this,
                scene,
                readonly,
                bezier_node,
                &path
            ](size_t i, Geom::Point Geom::Knot::* pref, QGraphicsItem* parent = nullptr) {
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
                    scene->addItem(e);
                    e->set_color({0xff, 0x66, 0x66});
                }
                auto point = path.knots[i].*pref;
                e->set_pos(point.x(), point.y());
                e->set_readonly(readonly);
                return e;
            };

            size_t i = 0;
            for (auto const& knot : path.knots) {
                auto pos = add_point_editor(i, &Geom::Knot::pos);
                add_point_editor(i, &Geom::Knot::tg1, pos);
                add_point_editor(i, &Geom::Knot::tg2, pos);
                knot_items.emplace_back(pos);

                if (!knot.uid.empty() && display_tags) {
                    auto e = scene->addText(util::str(knot.uid));
                    e->setX(knot.pos.x());
                    e->setY(knot.pos.y());
                    knot_items.emplace_back(e);
                }

                ++i;
            }
        }
    }
}

void BezierEditor::uninit() {
    old_size = -1;
    if (auto canvas = get_canvas()) {
        for (auto const& e : knot_items) {
            canvas->scene()->removeItem(e.get());
        }
        knot_items.clear();
    }
    curve_item.reset();
}

shared_ptr<core::BaseValue<Geom::BezierKnots>> BezierEditor::get_bezier_node() {
    return dynamic_pointer_cast<core::BaseValue<Geom::BezierKnots>>(get_node());
}

Geom::BezierKnots BezierEditor::get_path() {
    return get_bezier_node()->value(get_core_context());
}

} // namespace rainynite::studio
