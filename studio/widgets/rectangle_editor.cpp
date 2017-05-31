/*
 *  rectangle_editor.cpp - edit rectangles on canvas
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
#include <QGraphicsRectItem>

#include <geom_helpers/rectangle.h>

#include <widgets/canvas.h>
#include "rectangle_editor.h"

namespace studio {

RectangleEditor::RectangleEditor() {
}

RectangleEditor::~RectangleEditor() {
}

void RectangleEditor::set_canvas(Canvas* canvas) {
    CanvasEditor::set_canvas(canvas);
    rectangle_item.reset(canvas->scene()->addRect({}));
    update_position();
}

void RectangleEditor::set_node(std::shared_ptr<core::AbstractValue> node) {
    NodeEditor::set_node(node);
    update_position();
}

void RectangleEditor::time_changed(core::Time) {
    update_position();
}

void RectangleEditor::update_position() {
    if (rectangle_item == nullptr)
        return;
    if (auto node = dynamic_cast<core::BaseValue<Geom::Rectangle>*>(get_node().get())) {
        auto rectangle = node->get(get_time());
        rectangle_item->setRect(
            rectangle.pos.x(),
            rectangle.pos.y(),
            rectangle.size.x(),
            rectangle.size.y()
        );
    }
}

} // namespace studio
