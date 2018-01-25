/*  node_list.h - node list model
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

#ifndef STUDIO_MODELS_NODE_LIST_H_D9546412_036D_5DE7_AF03_B5FFEC71F437
#define STUDIO_MODELS_NODE_LIST_H_D9546412_036D_5DE7_AF03_B5FFEC71F437

#include <QAbstractItemModel>

#include <generic/context_listener.h>

namespace rainynite::studio {

class NodeListModel : public QAbstractItemModel, public ContextListener {
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

    /// Insert node at position
    void insert_node(core::NodeTreeIndex node, int position=-1);

    /**
     * Insert node only if it isn't already in model
     * @return true if node was added
     */
    bool insert_unique_node(core::NodeTreeIndex node, int position=-1);

    /// Get list of all nodes
//     vector<shared_ptr<core::AbstractValue>> const& get_all_nodes() const {
//         return nodes;
//     }
    shared_ptr<core::AbstractValue> get_node(QModelIndex const& index) const;
    core::NodeTreeIndex get_inner_index(QModelIndex const& index) const;

private:
    vector<core::NodeTreeIndex> nodes;
};

} // namespace rainynite::studio

#endif
