/*
 *  timeline_editor.cpp - timeline editing widgets
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
#include <core/node_info.h>
#include <core/node/abstract_list.h>

#include <widgets/timeline_area.h>
#include "node_editor.h"
#include "timeline_editor.h"

namespace studio {

TimelineEditor* add_timeline_editor(TimelineArea& canvas, TimelineEditorFactory const& factory) {
    auto editor = factory();
    if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
        context_listener->set_context(canvas.get_context());
    return canvas.add_editor(std::move(editor));
}

TimelineEditor* add_timeline_named_editor(TimelineArea& canvas, std::string const& name) {
    TimelineEditorFactory* factory;
    try {
        factory = &class_init::name_info<TimelineEditorFactory>(name);
    } catch (class_init::RuntimeTypeError const&) {
        // do something about it? should we really catch it?
        return nullptr;
    }
    return add_timeline_editor(canvas, *factory);
}

TimelineEditor* add_timeline_node_editor(TimelineArea& canvas, std::shared_ptr<core::AbstractValue> node) {
    if (node == nullptr)
        return nullptr;

    TimelineEditorFactory* factory;
    try {
        factory = &class_init::type_meta<TimelineEditorFactory>(node->get_type());
    } catch (class_init::RuntimeTypeError const&) {
        // do something about it? should we really catch it?
        return nullptr;
    }

    auto editor = add_timeline_editor(canvas, *factory);
    if (auto node_editor = dynamic_cast<NodeEditor*>(editor))
        node_editor->set_node(node);
    return editor;
}

} // namespace studio
