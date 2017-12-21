/*  node_model.cpp - node tree model wrapper
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

#include <fmt/format.h>

#include <core/node_info.h>
#include <core/node/make.h>
#include <core/node/abstract_node.h>
#include <core/nothing.h>
#include <core/serialize/node_writer.h>
#include <core/action_stack.h>
#include <core/document.h>
#include <core/actions/change_link.h>
#include <core/actions/custom_property.h>
#include <core/actions/set_enabled.h>
#include <core/actions/list.h>

#include <util/strings.h>
#include "node_model.h"

using namespace fmt::literals;

namespace rainynite::studio {

static const size_t MAX_VALUE_LENGTH = 32;

NodeModel::NodeModel(core::AbstractReference root_, shared_ptr<core::ActionStack> action_stack_, QObject* parent) :
    QAbstractItemModel(parent),
    root(root_),
    action_stack(action_stack_)
{
    if (auto document = dynamic_cast<core::Document*>(root.get())) {
        tree = document->get_tree();
    } else {
        tree = root_ ? make_shared<core::NodeTree>(root_, action_stack_) : nullptr;
    }
}

NodeModel::~NodeModel() {
}

void NodeModel::time_changed(core::Time time) {
    ContextListener::time_changed(time);
    Q_EMIT dataChanged(index(0, 1), index(2, 1));
}

QVariant NodeModel::data(QModelIndex const& index, int role) const {
    if (!index.isValid())
        return {};

    if (index.column() == 0) {
        switch (role) {
            case Qt::DisplayRole: {
                string node_role = "document";
                if (auto parent_node = get_node_as<core::AbstractNode>(parent(index)))
                    node_role = parent_node->get_name_at(index.row());
                else if (parent(index).isValid())
                    node_role = "{}"_format(index.row());
                string type_name = "<Bad node!>";
                if (auto node = get_node(index))
                    type_name = core::node_name(*node);
                return util::str("{}: {}"_format(node_role, type_name));
            } break;
            case Qt::ToolTipRole: {
                // TODO: node role documentation
                if (auto node = get_node_as<DocString>(index))
                    return util::str(node->doc_string());
                return {};
            } break;
            default:
                return {};
        }
    } else {
        switch (role) {
            case Qt::DisplayRole: {
                if (auto node = get_node(index)) {
                    auto value = node->get_any(get_core_context());
                    try {
                        auto s = core::serialize::value_to_string(value);
                        auto s_fixed = s.substr(0, MAX_VALUE_LENGTH);
                        auto s_fixed2 = s_fixed.substr(0, s_fixed.find('\n'));
                        if (s.size() > s_fixed2.size())
                            s_fixed2 += "...";
                        return util::str(s_fixed2);
                    } catch (class_init::RuntimeTypeError const& ex) {
                        return {};
                    }
                }
                return {};
            } break;
            default:
                return {};
        }
    }
}

Qt::ItemFlags NodeModel::flags(QModelIndex const&) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return {};
    switch (role) {
        case Qt::DisplayRole: {
            if (section == 0)
                return "node";
            else
                return "value";
        } break;
        default:
            return {};
    }
}

QModelIndex NodeModel::index(int row, int column, QModelIndex const& parent) const {
    return createIndex(row, column, (void*)get_inner_index(parent, row).get());
}

core::NodeTree::Index NodeModel::get_inner_index(QModelIndex const& parent, size_t i) const {
    if (tree == nullptr)
        return nullptr;
    if (!parent.isValid()) {
        if (i == 0)
            return tree->get_root_index();
        else
            return tree->get_null_index();
    } else {
        return tree->index(get_inner_index(parent), i);
    }
}

core::NodeTree::Index NodeModel::get_inner_index(QModelIndex const& index) const {
    if (tree == nullptr)
        return nullptr;
    if (!index.isValid()) {
        return tree->get_null_index();
    }
    return make_observer(static_cast<core::NodeTreeIndex*>(index.internalPointer()));
}

bool NodeModel::can_add_custom_property(QModelIndex const& parent) const {
    if (get_node_as<core::AbstractNode>(parent))
        return true;
    return false;
}

void NodeModel::add_empty_custom_property(QModelIndex const& parent, string const& name) {
    if (auto parent_node = get_node_as<core::AbstractNode>(parent)) {
        action_stack->emplace<core::actions::AddCustomProperty>(parent_node, name, core::make_value<Nothing>());
        // TODO: add rows
    }
}

bool NodeModel::is_custom_property(QModelIndex const& index) const {
    if (auto parent = get_node_as<core::AbstractNode>(index.parent())) {
        auto i = index.row();
        return parent->get_name_at(i)[0] == '_';
    }
    return false;
}

void NodeModel::remove_custom_property(QModelIndex const& index) {
    if (auto parent = get_node_as<core::AbstractNode>(index.parent())) {
        auto i = index.row();
        action_stack->emplace<core::actions::RemoveCustomProperty>(parent, parent->get_name_at(i));
        // TODO: remove rows
    }
}

bool NodeModel::can_add_element(QModelIndex const& parent) const {
    if (auto node = get_list_node(parent))
        return node->is_editable_list();
    return false;
}

void NodeModel::add_empty_element(QModelIndex const& parent) {
    if (auto node = get_list_node(parent)) {
        auto last = node->link_count();
        beginInsertRows(parent, last, last);
        action_stack->emplace<core::actions::ListPushNew>(node);
        endInsertRows();
        Q_EMIT dataChanged(index(last, 0, parent), index(last, 1, parent));
    }
}

void NodeModel::remove_list_item(QModelIndex const& parent, size_t index) {
    if (auto node = get_node_as<core::AbstractListLinked>(parent)) {
        if (node->is_editable_list()) {
            beginRemoveRows(parent, index, index);
            action_stack->emplace<core::actions::ListRemoveElement>(node, index);
            endRemoveRows();
        }
    }
}

void NodeModel::convert_node(QModelIndex const& index, core::NodeInfo const* node_info) {
    if (auto parent_node = get_list_node(index.parent())) {
        auto node = get_node(index);
        auto new_node = core::make_node_with_name<core::AbstractValue>(node_info->name(), node, get_core_context());

        replace_node(index, new_node);
    }
}

bool NodeModel::node_is_connected(QModelIndex const& index) const {
    if (auto node = get_node(index)) {
        size_t count = 0;
        return find_nodes<bool>(
            node,
            [&count]() -> optional<bool> {
                if (count > 0)
                    return make_optional(true);
                ++count;
                return {};
            }
        );
    }
    return false;
}

void NodeModel::disconnect_node(QModelIndex const& index) {
    replace_node(index, core::shallow_copy(*get_node(index)));
}

void NodeModel::connect_nodes(QList<QModelIndex> const& selection, QModelIndex const& source_index) {
    if (auto source = get_node(source_index)) {
        auto type = source->get_type();
        // TODO: check type of required by parent instead
        for (auto const& index : selection) {
            if (index != source_index && get_node(index)->get_type() == type) {
                replace_node(index, source);
            }
        }
    }
}

bool NodeModel::can_remove_node(QModelIndex const& index) const {
    if (auto node = get_node_as<core::AbstractListLinked>(index)) {
        if (node->link_count() == 0)
            return false;
        return index.parent().isValid() && get_link_type(index).accept(node->get_link(0)->get_type());
    }
    return false;
}

void NodeModel::remove_node(QModelIndex const& index) {
    if (auto node = get_node_as<core::AbstractListLinked>(index)) {
        auto child = node->get_link(0);
        replace_node(index, child);
    }
}

bool NodeModel::node_enabled(QModelIndex const& index) const {
    if (auto node = get_node(index)) {
        return node->enabled();
    }
    return false;
}

void NodeModel::node_set_enabled(QModelIndex const& index, bool value) {
    if (!action_stack)
        return;
    if (auto node = get_node(index)) {
        action_stack->emplace<core::actions::SetEnabled>(node, value);
    }
}

void NodeModel::replace_node(QModelIndex const& index, core::AbstractReference node) {
    if (!action_stack)
        return;
    if (auto parent = get_list_node(index.parent())) {
        // now tell Qt about our intentions
        size_t old_rows = 0;
        if (auto old_node = get_list_node(index))
            old_rows = old_node->link_count();
        size_t new_rows = 0;
        if (auto new_list = core::list_cast(node))
            new_rows = new_list->link_count();
        if (new_rows > old_rows)
            beginInsertRows(index, old_rows, new_rows-1);
        else if (new_rows < old_rows)
            beginRemoveRows(index, new_rows, old_rows-1);

        action_stack->emplace<core::actions::ChangeLink>(parent, index.row(), node);

        Q_EMIT dataChanged(index, index);

        if (new_rows > old_rows)
            endInsertRows();
        else if (new_rows < old_rows)
            endRemoveRows();
    }
}

void NodeModel::swap_nodes(QModelIndex const& a, QModelIndex const& b) {
    auto parent_a = get_list_node(a.parent());
    auto parent_b = get_list_node(b.parent());
    if (!parent_a || !parent_b)
        return;
    auto node_a = get_node(a);
    auto node_b = get_node(b);
    if (!node_a || !node_b)
        return;
    replace_node(a, node_b);
    replace_node(b, node_a);
}

bool NodeModel::can_move_up(size_t offset, QModelIndex const& /*parent*/) {
    return offset > 0;
}

bool NodeModel::can_move_down(size_t offset, QModelIndex const& parent) {
    return offset < get_list_node(parent)->link_count()-1;
}

void NodeModel::move_up(size_t offset, QModelIndex const& parent) {
    swap_nodes(index(offset, 0, parent), index(offset-1, 0, parent));
}

void NodeModel::move_down(size_t offset, QModelIndex const& parent) {
    swap_nodes(index(offset, 0, parent), index(offset+1, 0, parent));
}

core::TypeConstraint NodeModel::get_link_type(QModelIndex const& index) const {
    if (auto parent = get_node_as<core::AbstractListLinked>(index.parent()))
        return parent->get_link_type(get_node_index(index));
    return {};
}

core::AbstractReference NodeModel::get_node(QModelIndex const& index) const {
    if (!index.isValid())
        return nullptr;
    if (!index.parent().isValid())
        return root;
    if (auto pnode = get_node(index.parent())) {
        if (auto parent_node = dynamic_pointer_cast<core::AbstractListLinked>(pnode))
            if ((size_t) index.row() < parent_node->link_count())
                return parent_node->get_link(index.row());
    }
    return nullptr;
}

size_t NodeModel::get_node_index(QModelIndex const& index) const {
    return index.row();
}

QModelIndex NodeModel::parent(QModelIndex const& index) const {
    if (!index.isValid())
        return {};
    if (auto id = get_inner_index(index)) {
        if (auto parent = id->parent) {
            if (!parent->null())
                return createIndex(parent->index, 0, (void*)parent.get());
        }
        return {};
    }
    return {};
}

int NodeModel::rowCount(QModelIndex const& parent) const {
    if (auto id = get_inner_index(parent))
        return tree->children_count(id);
    return 0;
}

int NodeModel::columnCount(QModelIndex const& /*parent*/) const {
    return 2;
}

} // namespace rainynite::studio
