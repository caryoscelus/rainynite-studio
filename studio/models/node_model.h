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

#include <core/node_info.h>
#include <core/node/traverse.h>

#include <generic/context_listener.h>

namespace rainynite::studio {

class NodeModel : public QAbstractItemModel, public ContextListener
{
    Q_OBJECT

public:
    explicit NodeModel(core::AbstractReference root, shared_ptr<core::ActionStack> action_stack, QObject* parent = nullptr);
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
    bool removeRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;

public:
    void time_changed(core::Time time) override;

public:
    bool can_add_custom_property(QModelIndex const& parent) const;
    void add_empty_custom_property(QModelIndex const& parent, std::string const& name);
    bool is_custom_property(QModelIndex const& index) const;
    void remove_custom_property(QModelIndex const& index);
    bool can_add_element(QModelIndex const& parent) const;
    void add_empty_element(QModelIndex const& parent);
    void convert_node(QModelIndex const& index, core::NodeInfo const* node_info);
    bool node_is_connected(QModelIndex const& index) const;
    void disconnect_node(QModelIndex const& index);
    void connect_nodes(QList<QModelIndex> const& selection, QModelIndex const& source);
    void replace_node(QModelIndex const& index, core::AbstractReference node);
    void swap_nodes(QModelIndex const& a, QModelIndex const& b);
    bool can_remove_node(QModelIndex const& index) const;
    /**
     * Remove "filter" node (replacing it with its child)
     */
    void remove_node(QModelIndex const& index);
    /**
     * Checks if node can be moved up.
     *
     * NOTE: this function doesn't check type compatibility! It should usually
     * be used after parent is checked to be editable list.
     */
    bool can_move_up(size_t offset, QModelIndex const& parent);
    /**
     * Checks if node can be moved down, same as can_move_up.
     */
    bool can_move_down(size_t offset, QModelIndex const& parent);
    void move_up(size_t offset, QModelIndex const& parent);
    void move_down(size_t offset, QModelIndex const& parent);

public:
    template <typename T, typename F>
    T find_nodes(core::AbstractReference node_to_find, F f) const {
        // TODO: perfect forward f
        return core::traverse_once<T>(
            root,
            [this, &f, &node_to_find](auto const& node) -> optional<T> {
                if (node == node_to_find)
                    return f();
                return {};
            },
            core::TraverseDepth::Deeper
        );
    }

public:
    optional<std::type_index> get_link_type(QModelIndex const& index) const;
    shared_ptr<core::AbstractValue> get_node(QModelIndex const& index) const;
    template <class T>
    shared_ptr<T> get_node_as(QModelIndex const& index) const {
        return dynamic_pointer_cast<T>(get_node(index));
    }
    inline shared_ptr<core::AbstractListLinked> get_list_node(QModelIndex const& index) const {
        return get_node_as<core::AbstractListLinked>(index);
    }

    size_t get_node_index(QModelIndex const& index) const;

private:
    quintptr get_id(QModelIndex const& parent, size_t i) const;

private:
    core::AbstractReference root;
    shared_ptr<core::ActionStack> action_stack;
private:
    mutable map<pair<QModelIndex, size_t>, quintptr> indexes;
    mutable map<quintptr, QModelIndex> parents;
    mutable quintptr index_count = 0;
};

} // namespace rainynite::studio

#endif
