/*  point_editor.cpp - edit points on canvas
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

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>

#include <2geom/point.h>

#include <core/action_stack.h>
#include <core/actions/change_value.h>

#include <widgets/canvas.h>
#include "point_item.h"
#include "point_editor.h"

namespace rainynite::studio {

PointEditor::PointEditor() {
}

PointEditor::~PointEditor() {
}

void PointEditor::setup_canvas() {
    point_item = make_unique<PointItem>(
        [this](double x, double y) {
            save_position(x, y);
        }
    );
    get_scene()->addItem(point_item.get());
    update_position();
}

void PointEditor::node_update() {
    update_position();
}

void PointEditor::time_changed(core::Time time) {
    ContextListener::time_changed(time);
    update_position();
}

void PointEditor::update_position() {
    if (point_item == nullptr)
        return;
    if (auto maybe_point = get_value<Geom::Point>()) {
        auto point = *maybe_point;
        point_item->set_pos(point.x(), point.y());
        point_item->set_readonly(!get_node()->is_const());
    }
}

void PointEditor::save_position(double x, double y) {
    if (auto node = get_node_as<Geom::Point>()) {
        if (node->can_set()) {
            auto action_stack = get_context()->action_stack();
            action_stack->emplace<core::actions::ChangeValue>(
                node,
                Geom::Point{x, y}
            );
            action_stack->close();
        }
    }
}

} // namespace rainynite::studio
