/*  editor.cpp - abstract canvas editor
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

#include <QDebug>

#include <core/node_info/node_info.h>
#include <core/node_tree/exceptions.h>

#include <generic/node_editor.h>
#include "abstract_editor.h"
#include "registry.h"

namespace rainynite::studio {

shared_ptr<AbstractCanvasEditor> add_canvas_named_editor(AbstractCanvas& canvas, string const& name) {
    try {
        shared_ptr<AbstractCanvasEditor> editor = class_init::name_info<AbstractAbstractCanvasEditorFactory>(name)();
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        canvas.add_editor(editor);
        return editor;
    } catch (class_init::RuntimeTypeError const&) {
        return nullptr;
    }
}

vector<shared_ptr<AbstractCanvasEditor>> add_canvas_node_editor(AbstractCanvas& canvas, core::NodeTree::Index index) {
    auto node = canvas.get_context()->get_node(index);
    if (node == nullptr)
        return {};

    vector<shared_ptr<AbstractCanvasEditor>> added_editors;
    shared_ptr<AbstractCanvasEditor> editor;
    try {
        editor = make_canvas_editor_for(canvas, node->get_type());
    } catch (class_init::RuntimeTypeError const&) {
    }

    if (editor != nullptr) {
        canvas.add_editor(editor);
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        if (auto node_editor = dynamic_cast<NodeEditor*>(editor.get()))
            node_editor->set_node(index);
        added_editors.push_back(editor);
    }

    editor.reset();
    try {
        editor = make_canvas_node_editor_for(canvas, typeid(*node));
    } catch (class_init::TypeLookupError const&) {
    }

    if (editor != nullptr) {
        // TODO: avoid code duplication
        canvas.add_editor(editor);
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(canvas.get_context());
        if (auto node_editor = dynamic_cast<NodeEditor*>(editor.get()))
            node_editor->set_node(index);
        added_editors.push_back(editor);
    }

    bool show_children = false;
    try {
        show_children = class_init::name_info<NodeEditorShowChildren>(core::node_name(*node))();
    } catch (class_init::TypeLookupError const&) {
    }

    if (show_children) {
        // NOTE: this may lead to infinite recursion if node tree is looped
        if (auto tree = canvas.get_context()->tree()) {
            for (size_t i = 0; i < tree->children_count(index); ++i) {
                core::NodeTree::Index child_idx;
                try {
                    child_idx = tree->index(index, i);
                } catch (core::NodeTreeError const&) {
                    // TODO: better report
                    qWarning() << "adding child failed";
                    break;
                }
                auto children_editors = add_canvas_node_editor(canvas, child_idx);
                added_editors.insert(
                    added_editors.end(),
                    children_editors.begin(),
                    children_editors.end()
                );
            }
        }
    }

    return added_editors;
}

} // namespace rainynite::studio
