/*  editor_context.cpp - editor Context
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

#include <core/document.h>

#include "editor_context.h"

namespace rainynite::studio {

void EditorContext::set_active_node(core::NodeTree::Index index) {
    if (index == active_node)
        return;
    active_node = index;
    changed_active_node()(index);
}

shared_ptr<core::NodeTree> EditorContext::tree() const {
    if (auto document = get_context()->get_document()) {
        return document->get_tree();
    }
    return nullptr;
}

shared_ptr<core::ActionStack> EditorContext::action_stack() const {
    if (auto document = get_context()->get_document())
        return document->get_action_stack();
    // TODO: check whether it's ok to return nullptr here
    return nullptr;
}

shared_ptr<EditorContext> global_dummy_context() {
    static auto context_instance = make_shared<core::Context>();
    static auto instance = make_shared<EditorContext>(context_instance);
    return instance;
}

} // namespace rainynite::studio
