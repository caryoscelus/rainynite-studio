/*  node_editor.cpp - abstract node editor widget class
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

#include <core/node_tree/transform.h>
#include <core/action_stack.h>
#include <core/util/nullptr.h>

#include "node_editor.h"

namespace rainynite::studio {

void NodeEditor::set_node(core::NodeTreeIndex index) {
    if (node_connection.connected())
        node_connection.disconnect();
    node_index = index;
    auto node = get_context()->get_node(node_index);
    if (node == nullptr)
        return;
    node_update();
    node_connection = node->subscribe([this]() {
        node_update();
    });
}

Geom::Affine get_transform(NodeEditor const& editor) {
    if (auto ctx = editor.get_context()) {
        if (auto tree = ctx->tree())
            return core::get_transform(ctx->get_context(), *tree, editor.get_node_index());
    }
    return {};
}

void NodeEditor::write_value(any value) {
    auto action_stack = no_null(get_context()->action_stack());
    action_stack->emplace<core::actions::ChangeValueAt>(
        get_node(),
        value,
        get_core_context()
    );
}

void NodeEditor::close_action() {
    auto action_stack = no_null(get_context()->action_stack());
    action_stack->close();
}

} // namespace rainynite::studio
