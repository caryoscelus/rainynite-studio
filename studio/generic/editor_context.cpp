/*
 *  editor_context.cpp - editor Context
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

#include "editor_context.h"

namespace studio {

void EditorContext::set_active_node(std::shared_ptr<core::AbstractValue> node) {
    if (node == active_node)
        return;
    active_node = node;
    changed_active_node()(node);
}

std::shared_ptr<EditorContext> global_dummy_context() {
    static auto context_instance = std::make_shared<core::Context>();
    static auto instance = std::make_shared<EditorContext>(context_instance);
    return instance;
}

} // namespace studio