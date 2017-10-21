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

#include <core/action_stack.h>
#include <core/actions/change_value.h>

#include <util/strings.h>
#include <util/pen.h>
#include <widgets/canvas.h>
#include "qt_path.h"
#include "point_item.h"
#include "bezier_editor.h"

using namespace fmt::literals;

namespace rainynite::studio {

BezierEditor::BezierEditor() {
    init();
}

BezierEditor::~BezierEditor() {
    uninit();
}

void BezierEditor::setup_canvas() {
    uninit();
    init();
}

void BezierEditor::node_update() {
    redraw();
}

void BezierEditor::time_changed(core::Time) {
    redraw();
}

void BezierEditor::redraw() {
    if (get_scene()) {
        auto path = get_path();
        // TODO
        if (path.size() == old_size) {
            curve_item->setPath(path_to_qt(path));
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

            curve_item.reset(scene->addPath(path_to_qt(path)));
            curve_item->setPen(pens::cosmetic_dash());

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
                if (parent)
                    e->setParentItem(parent);
                else
                    scene->addItem(e);
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

                if (!knot.uid.empty()) {
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
    if (auto canvas = get_canvas()) {
        for (auto const& e : knot_items) {
            canvas->scene()->removeItem(e.get());
        }
        knot_items.clear();
    }
    curve_item.reset();
}

}
