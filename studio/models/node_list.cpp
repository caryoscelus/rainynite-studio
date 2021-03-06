/*  node_list.cpp - node list model
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

#include <core/node_info/node_info.h>
#include <core/node_tree/exceptions.h>
#include <core/util/nullptr.h>

#include <util/strings.h>
#include "node_list.h"

namespace rainynite::studio {

NodeListModel::NodeListModel(QObject* parent) :
    QAbstractItemModel(parent)
{
}

NodeListModel::~NodeListModel() {
}

QVariant NodeListModel::data(QModelIndex const& index, int role) const {
    if (!index.isValid() || index.column() != 0 || index.parent().isValid())
        return {};

    try {
        switch (role) {
            case Qt::DisplayRole:
                return util::str(core::node_name(*no_null(get_node(index))));
            default:
                return {};
        }
    } catch (core::NodeTreeError const&) {
        // TODO: report
        return {};
    } catch (...) {
        return {};
    }
}

Qt::ItemFlags NodeListModel::flags(QModelIndex const& /*index*/) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant NodeListModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const {
    return {};
}

QModelIndex NodeListModel::index(int row, int column, QModelIndex const& parent) const {
    if (parent.isValid() || column != 0)
        return {};
    return createIndex(row, column, nullptr);
}

QModelIndex NodeListModel::parent(QModelIndex const& /*index*/) const {
    return {};
}

int NodeListModel::rowCount(QModelIndex const& parent) const {
    if (parent.isValid())
        return 0;
    return nodes.size();
}

int NodeListModel::columnCount(QModelIndex const& /*parent*/) const {
    return 1;
}

bool NodeListModel::removeRows(int row, int count, QModelIndex const& parent) {
    if (parent.isValid())
        return false;
    beginRemoveRows(parent, row, row+count-1);
    nodes.erase(nodes.begin()+row, nodes.begin()+row+count);
    endRemoveRows();
    return true;
}

void NodeListModel::insert_node(core::NodeTreePath const& node, int position) {
    if (position < 0)
        position = nodes.size();
    beginInsertRows({}, position, position+1);
    nodes.insert(nodes.begin()+position, node);
    endInsertRows();
}

bool NodeListModel::insert_unique_node(core::NodeTreeIndex idx, int position) {
    try {
        auto path = tree_index_to_path(*get_context()->tree(), idx);
        if (std::find(nodes.begin(), nodes.end(), path) == nodes.end()) {
            insert_node(path, position);
            return true;
        }
    } catch (...) {
        // TODO
    }

    return false;
}

shared_ptr<core::AbstractValue> NodeListModel::get_node(QModelIndex const& index) const {
    return get_context()->get_node(get_inner_index(index));
}

core::NodeTreeIndex NodeListModel::get_inner_index(QModelIndex const& index) const {
    try {
        return tree_path_to_index(*get_context()->tree(), nodes[index.row()]);
    } catch (...) {
        return {};
    }
}

} // namespace rainynite::studio
