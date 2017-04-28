/*
 *  node_model.h - node tree model wrapper
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

#ifndef __STUDIO__NODE_MODEL_H_E924C400
#define __STUDIO__NODE_MODEL_H_E924C400

#include <QAbstractItemModel>

#include <core/node.h>

namespace studio {

class NodeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NodeModel(core::AbstractReference root, QObject* parent = 0);
    virtual ~NodeModel();

    QVariant data(QModelIndex const& index, int role) const override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      QModelIndex const& parent = QModelIndex()) const override;
    QModelIndex parent(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

public:
    core::AbstractReference get_node(QModelIndex const& index) const;

private:
    quintptr get_id(core::AbstractReference ref, QModelIndex const& parent = QModelIndex()) const;

private:
    core::AbstractReference root;
private:
    mutable std::map<quintptr, std::weak_ptr<core::AbstractValue>> pointers;
    mutable std::map<core::AbstractValue*, quintptr> indexes;
    mutable std::map<quintptr, QModelIndex> parents;
    mutable quintptr last_index = 0;
};

} // namespace studio

#endif