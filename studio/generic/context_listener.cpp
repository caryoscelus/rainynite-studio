/*  context_listener.cpp - Context-dependent entity
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

#include <core/context.h>
#include <core/action_stack.h>

#include "context_listener.h"

namespace rainynite::studio {

ContextListener::ContextListener(shared_ptr<EditorContext> context_) {
    set_context(context_);
}

void ContextListener::set_context(shared_ptr<EditorContext> context_) {
    // TODO: disconnect from previous context!
    context = context_;
    if (context == nullptr)
        context = global_dummy_context();
    connect_boost(
        context->changed_time(),
        [this](core::Time time) {
            time_changed(time);
        }
    );
    connect_boost(
        context->changed_fps(),
        [this](core::Time::fps_type fps) {
            fps_changed(fps);
        }
    );
    connect_boost(
        context->changed_active_node(),
        [this](auto index) {
            active_node_index_changed(index);
        }
    );
    time_changed(get_core_context()->get_time());
    fps_changed(get_core_context()->get_fps());
}

void ContextListener::undo() {
    get_context()->action_stack()->undo();
}

void ContextListener::redo() {
    get_context()->action_stack()->redo();
}

void ContextListener::clear_undo_history() {
    get_context()->action_stack()->clear();
}

void ContextListener::set_active_node(core::NodeTreeIndex index) {
    get_context()->set_active_node(index);
}

void ContextListener::active_node_index_changed(core::NodeTreeIndex index) {
    active_node_changed(get_context()->get_node(index));
}

} // namespace rainynite::studio
