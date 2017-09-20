/*  editor.cpp - abstract canvas editor
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

#include <core/node_info.h>

#include <generic/node_editor.h>
#include "editor.h"
#include "registry.h"

namespace rainynite::studio {

shared_ptr<CanvasEditor> add_canvas_named_editor(AbstractCanvas& canvas, string const& name) {
    try {
        shared_ptr<CanvasEditor> editor = class_init::name_info<AbstractCanvasEditorFactory>(name)();
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        canvas.add_editor(editor);
        return editor;
    } catch (class_init::RuntimeTypeError const&) {
        return nullptr;
    }
}

shared_ptr<CanvasEditor> add_canvas_node_editor(AbstractCanvas& canvas, shared_ptr<core::AbstractValue> node) {
    if (node == nullptr)
        return nullptr;

    shared_ptr<CanvasEditor> editor = nullptr;
    try {
        editor = make_canvas_editor_for(canvas, node->get_type());
    } catch (class_init::RuntimeTypeError const&) {
    }

    if (editor) {
        if (auto node_editor = dynamic_cast<NodeEditor*>(editor.get()))
            node_editor->set_node(node);
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        canvas.add_editor(editor);
    }

    bool show_children = false;
    try {
        show_children = class_init::name_info<NodeEditorShowChildren>(core::node_name(*node))();
    } catch (class_init::TypeLookupError const&) {
    }

    if (show_children) {
        // NOTE: this may lead to infinite recursion if node tree is looped
        if (auto parent = dynamic_cast<core::AbstractListLinked*>(node.get())) {
            for (auto child : parent->get_links())
                add_canvas_node_editor(canvas, child);
        }
    }

    return editor;
}

} // namespace rainynite::studio
