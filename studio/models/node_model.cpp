/*
 *  node_model.cpp - node tree model wrapper
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

#include "node_model.h"

namespace studio {

NodeModel::NodeModel(core::AbstractReference root_, QObject* parent) :
    QAbstractItemModel(parent),
    root(root_)
{
}

NodeModel::~NodeModel() {
}

QVariant NodeModel::data(QModelIndex const& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (auto node = get_node(index))
        return QString::fromStdString(core::node_name(node));

    return "<Bad node!>";
}

Qt::ItemFlags NodeModel::flags(QModelIndex const& index) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return "header";
}

QModelIndex NodeModel::index(int row, int column, QModelIndex const& parent) const {
    if (parent == QModelIndex()) {
        if (row == 0 && column == 0)
            return createIndex(row, column, get_id(root));
        return QModelIndex();
    }
    auto pnode = get_node(parent);
    if (auto parent_node = std::dynamic_pointer_cast<core::AbstractListLinked>(pnode)) {
        if (row >= parent_node->link_count())
            return QModelIndex();
        auto node = parent_node->get_links()[row];
        return createIndex(row, column, get_id(node, parent));
    }
    return QModelIndex();
}

quintptr NodeModel::get_id(core::AbstractReference ref, QModelIndex const& parent) const {
    auto raw = ref.get();
    if (indexes.count(raw))
        return indexes.at(raw);
    auto index = last_index++;
    pointers.emplace(index, ref);
    indexes.emplace(raw, index);
    parents.emplace(index, parent);
    return index;
}

core::AbstractReference NodeModel::get_node(QModelIndex const& index) const {
    if (index == QModelIndex())
        return nullptr;
    if (index.parent() == QModelIndex())
        return root;
    if (auto pnode = get_node(index.parent())) {
        if (auto parent_node = std::dynamic_pointer_cast<core::AbstractListLinked>(pnode))
            return parent_node->get_links()[index.row()];
    }
    return nullptr;
}

QModelIndex NodeModel::parent(QModelIndex const& index) const {
    if (index == QModelIndex())
        return QModelIndex();
    auto id = index.internalId();
    return parents.at(id);
}

int NodeModel::rowCount(QModelIndex const& parent) const {
    if (auto value = get_node(parent)) {
        if (auto node = std::dynamic_pointer_cast<core::AbstractListLinked>(value))
            return node->link_count();
        return 0;
    }
    if (root)
        return 1;
    return 0;
}

int NodeModel::columnCount(QModelIndex const& parent) const {
    return 1;
}

} // namespace studio
