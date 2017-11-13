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
                if (auto node = get_node_as<core::DocString>(index))
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
    if (parent == QModelIndex()) {
        if (row == 0 && column == 0)
            return createIndex(0, 0, get_id({}, 0));
        return QModelIndex();
    }
    if (auto parent_node = get_list_node(parent)) {
        if ((size_t) row >= parent_node->link_count())
            return QModelIndex();
        return createIndex(row, column, get_id(parent, row));
    }
    return QModelIndex();
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
        beginInsertRows(parent, last-1, last-1);
        action_stack->emplace<core::actions::ListPushNew>(node);
        endInsertRows();
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
    if (auto node = get_node_as<core::AbstractNodeBase>(index)) {
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

bool NodeModel::removeRows(int row, int count, QModelIndex const& parent) {
    if (auto parent_node = get_list_node(parent)) {
        beginRemoveRows(parent, row, row+count-1);
        for (int i = 0; i < count; ++i)
            action_stack->emplace<core::actions::ListRemoveElement>(parent_node, row+i);
        endRemoveRows();
        return true;
    }
    return false;
}

quintptr NodeModel::get_id(QModelIndex const& parent, size_t i) const {
    pair<QModelIndex,size_t> parent_and_index = {parent, i};
    auto found = indexes.find(parent_and_index);
    if (found != indexes.end())
        return found->second;
    auto interal_id = index_count++;
    indexes.emplace(parent_and_index, interal_id);
    parents.emplace(interal_id, parent);
    return interal_id;
}

core::TypeConstraint NodeModel::get_link_type(QModelIndex const& index) const {
    if (auto parent = get_node_as<core::AbstractListLinked>(index.parent()))
        return parent->get_link_type(get_node_index(index));
    return {};
}

core::AbstractReference NodeModel::get_node(QModelIndex const& index) const {
    if (index == QModelIndex())
        return nullptr;
    if (index.parent() == QModelIndex())
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
    if (index == QModelIndex())
        return QModelIndex();
    auto id = index.internalId();
    return parents.at(id);
}

int NodeModel::rowCount(QModelIndex const& parent) const {
    if (auto value = get_node(parent)) {
        if (auto node = dynamic_pointer_cast<core::AbstractListLinked>(value))
            return node->link_count();
        return 0;
    }
    if (root)
        return 1;
    return 0;
}

int NodeModel::columnCount(QModelIndex const& /*parent*/) const {
    return 2;
}

} // namespace rainynite::studio
