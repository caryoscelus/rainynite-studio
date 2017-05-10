/*
 *  bezier_editor.cpp - edit beziers on canvas
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

#include <2geom/point.h>

#include <widgets/canvas.h>
#include "point_editor.h"

namespace studio {

PointEditor::PointEditor() {
}

PointEditor::~PointEditor() {
}

void PointEditor::set_canvas(Canvas* canvas) {
    CanvasEditor::set_canvas(canvas);
    point_item.reset(canvas->scene()->addEllipse(0, 0, 4, 4));
    update_position();
}

void PointEditor::set_node(std::shared_ptr<core::AbstractValue> node) {
    NodeEditor::set_node(node);
    update_position();
}

void PointEditor::time_changed(core::Time) {
    update_position();
}

void PointEditor::update_position() {
    if (point_item == nullptr)
        return;
    if (auto node = dynamic_cast<core::BaseValue<Geom::Point>*>(get_node().get())) {
        auto point = node->get(get_time());
        point_item->setPos({point.x()-2, point.y()-2});
    }
}

} // namespace studio
