/*
 *  node_list.h - node list model
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

#ifndef __STUDIO__MODELS__NODE_LIST_H__CE0F8EE4
#define __STUDIO__MODELS__NODE_LIST_H__CE0F8EE4

#include <QAbstractItemModel>

namespace rainynite::studio {

class NodeListModel : public QAbstractItemModel
{
public:
    explicit NodeListModel(QObject* parent = nullptr);
    virtual ~NodeListModel();

public:
    QVariant data(QModelIndex const& index, int role) const override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, QModelIndex const& parent={}) const override;
    QModelIndex parent(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent={}) const override;
    int columnCount(QModelIndex const& parent={}) const override;

public:
    bool removeRows(int row, int count, QModelIndex const& parent={}) override;
    void insert_node(shared_ptr<core::AbstractValue> node, int position=-1);
    void insert_unique_node(shared_ptr<core::AbstractValue> node, int position=-1);
    vector<shared_ptr<core::AbstractValue>> const& get_all_nodes() const {
        return nodes;
    }
    shared_ptr<core::AbstractValue> get_node(QModelIndex const& index) const;

private:
    vector<shared_ptr<core::AbstractValue>> nodes;
};

} // namespace rainynite::studio

#endif
