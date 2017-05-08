/*
 *  canvas_editor.cpp - on-canvas value edit/display "widgets"
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

#include <core/class_init.h>
#include <core/node.h>

#include <widgets/canvas.h>
#include "node_editor.h"
#include "canvas_editor.h"

namespace studio {

void CanvasEditor::set_canvas(Canvas* canvas_) {
    if (canvas != canvas_) {
        canvas = canvas_;
    }
}

void add_canvas_editor(Canvas& canvas, std::shared_ptr<core::AbstractValue> node) {
    if (node == nullptr)
        return;
    try {
        auto editor = class_init::type_info<CanvasEditorFactory,std::unique_ptr<CanvasEditor>>(node->get_type());
        if (auto node_editor = dynamic_cast<NodeEditor*>(editor.get()))
            node_editor->set_node(node);
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        canvas.add_node_editor(std::move(editor));
    } catch (class_init::RuntimeTypeError const&) {
        // do something about it? should we really catch it?
    }
}

} // namespace studio
