/*
 *  rectangle_editor.h - edit rectangles on canvas
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

#ifndef __STUDIO__WIDGETS__RECTANGLE_EDITOR_H__F5CE7038
#define __STUDIO__WIDGETS__RECTANGLE_EDITOR_H__F5CE7038

#include <core/std/memory.h>

#include <generic/node_editor.h>
#include <canvas/editor.h>
#include <generic/context_listener.h>

class QGraphicsRectItem;

namespace rainynite::studio {

class RectangleEditor : public NodeEditor, public CanvasEditor {
public:
    RectangleEditor();
    virtual ~RectangleEditor();
public:
    void setup_canvas() override;
    void node_update() override;
    void time_changed(core::Time time) override;
private:
    void update_position();
    unique_ptr<QGraphicsRectItem> rectangle_item;
};

} // namespace rainynite::studio

#endif
