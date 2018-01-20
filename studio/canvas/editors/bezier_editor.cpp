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
#include <QMouseEvent>
#include <QInputDialog>
#include <QLineEdit>

#include <core/actions/change_value.h>
#include <core/util/nullptr.h>
#include <core/node_tree/transform.h>

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
    if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
        auto pos = convert_pos(mouse_event->pos());
        if (mouse_event->type() == QEvent::MouseButtonPress) {
            if (mouse_event->button() == Qt::LeftButton) {
                drawing = true;
                if (auto node = get_bezier_node()) {
                    if (node->can_set_any_at()) {
                        auto path = get_path();
                        path.emplace_back(pos);
                        write_value(path);
                        add_knot_editor(path.size()-1);
                        reset_curve(path);
                    }
                }
            }
        }
    }
    return false;
}

Geom::Point BezierEditor::convert_pos(QPoint const& src) const {
    auto affine = get_transform(*this);
    return util::point(no_null(get_canvas())->mapToScene(src)) * affine.inverse();
}

void BezierEditor::node_update() {
    redraw();
}

void BezierEditor::time_changed(core::Time) {
    redraw();
}

bool BezierEditor::is_readonly() const {
    return !get_scene() || !get_bezier_node() || !get_bezier_node()->can_set_any_at();
}

void BezierEditor::redraw() {
    if (get_scene() && get_bezier_node()) {
        auto path = get_path();
        // NOTE: this is to avoid full redraw while editing
        // TODO: make sure it gets properly updated on non-editing changes
        if ((ptrdiff_t)path.size() == old_size) {
            reset_curve(path);
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

    if (bezier_node->can_set_any_at()) {
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

Geom::Point& flag_to_ref(Geom::Knot& source, BezierEditor::BezierPointFlag flag) {
    switch (flag) {
        case BezierEditor::Point: return source.pos;
        case BezierEditor::Tg1: return source.tg1;
        case BezierEditor::Tg2: return source.tg2;
        default: throw std::logic_error("Invalid BezierPointFlag value");
    }
}

void BezierEditor::point_moved(size_t point_id, QPointF const& pos) {
    auto bezier_node = no_null(get_bezier_node());
    auto path = bezier_node->mod();

    auto [flag, i] = point_id_to_fn(point_id);

    auto& point = flag_to_ref(path.knots[i], flag);
    point = util::point(pos);

    // currently only enabled symmetric while appending..
    if (appending && flag > Point) {
        auto opposite = flag == Tg1 ? Tg2 : Tg1;
        auto& point_s = flag_to_ref(path.knots[i], opposite);
        point_s = -point;
    }

    write_value(path);
}

void BezierEditor::point_stopped_moving(size_t /*point_id*/) {
    close_action();
}

void BezierEditor::point_double_clicked(size_t point_id) {
    if (is_readonly())
        return;
    // TODO: different action for tangent points
    auto path = get_path();
    auto [_, i] = point_id_to_fn(point_id);
    auto& point = path.knots[i];
    auto tag = QInputDialog::getText(nullptr, "Bezier point tag", "Edit bezier point tag", QLineEdit::Normal, util::str(point.uid));
    point.uid = util::str(tag);
    write_value(path);
}

QGraphicsItem* BezierEditor::add_point_editor(size_t i, BezierPointFlag what, QGraphicsItem* parent) {
    auto e = new ListenerPointItem(this, point_id_from_fn(what, i));
    if (parent) {
        e->setParentItem(parent);
        e->set_color({0xff, 0xff, 0x88});
    } else {
        e->setParentItem(no_null(curve_item.get()));
        e->set_color({0xff, 0x66, 0x66});
    }
    auto path = get_path();
    auto point = flag_to_ref(path.knots[i], what);
    e->set_pos(point.x(), point.y());
    e->set_readonly(false);
    return e;
}

void BezierEditor::add_knot_editor(size_t i) {
    auto pos = add_point_editor(i, Point);
    add_point_editor(i, Tg1, pos);
    add_point_editor(i, Tg2, pos);
    knot_items.emplace_back(pos);
}

void BezierEditor::reset_curve(Geom::BezierKnots const& path) {
    auto scene = no_null(get_scene());
    if (!curve_item)
        curve_item.reset(scene->addPath(util::path_to_qt(path)));
    else
        curve_item->setPath(util::path_to_qt(path));
    curve_item->setPen(curve_pen);
    auto affine = get_transform(*this);
    curve_item->setTransform(QTransform{util::matrix(affine)});
}

void BezierEditor::uninit() {
    old_size = -1;
    for (auto const& e : knot_items) {
        e->setParentItem(nullptr);
    }
    if (auto scene = get_scene()) {
        for (auto const& e : knot_items)
            scene->removeItem(e.get());
        scene->removeItem(curve_item.get());
    }
    knot_items.clear();
    remove_tags();
    curve_item.reset();
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

pair<BezierEditor::BezierPointFlag, size_t> BezierEditor::point_id_to_fn(size_t point_id) const {
    return {
        (BezierPointFlag) (point_id % BezierPointFlag::Count),
        point_id / BezierPointFlag::Count
    };
}

size_t BezierEditor::point_id_from_fn(BezierPointFlag flag, size_t n) const {
    return n*BezierPointFlag::Count+flag;
}

} // namespace rainynite::studio
