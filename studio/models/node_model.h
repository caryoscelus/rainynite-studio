/*  node_model.h - node tree model wrapper
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

#ifndef STUDIO_MODELS_NODE_MODEL_H_7049227A_5FEA_5FDF_9D8E_5C61B5F8AAAF
#define STUDIO_MODELS_NODE_MODEL_H_7049227A_5FEA_5FDF_9D8E_5C61B5F8AAAF

#include <QAbstractItemModel>

#include <core/node_info/node_info.h>
#include <core/node/traverse.h>
#include <core/node_tree/node_tree.h>

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
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, QModelIndex const& parent = {}) const override;
    QModelIndex parent(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent = {}) const override;
    int columnCount(QModelIndex const& parent = {}) const override;

    void time_changed(core::Time time) override;

    bool can_add_custom_property(QModelIndex const& parent) const;
    void add_empty_custom_property(QModelIndex const& parent, std::string const& name);
    bool is_custom_property(QModelIndex const& index) const;
    void remove_custom_property(QModelIndex const& index);
    bool can_add_element(QModelIndex const& parent) const;
    void add_empty_element(QModelIndex const& parent);
    void remove_list_item(QModelIndex const& parent, size_t index);
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

    /// Return node enabled status
    bool node_enabled(QModelIndex const& index) const;

    /// Enable/disable node
    void node_set_enabled(QModelIndex const& index, bool value);

    template <typename T, typename F>
    T find_nodes(core::AbstractReference node_to_find, F f) const {
        // TODO: perfect forward f
        return core::traverse_once<T>(
            root,
            [&f, &node_to_find](auto const& node) -> optional<T> {
                if (node == node_to_find)
                    return f();
                return {};
            },
            core::TraverseDepth::Deeper
        );
    }

    core::TypeConstraint get_link_type(QModelIndex const& index) const;
    shared_ptr<core::AbstractValue> get_node(QModelIndex const& index) const;
    template <class T>
    shared_ptr<T> get_node_as(QModelIndex const& index) const {
        return dynamic_pointer_cast<T>(get_node(index));
    }
    shared_ptr<core::AbstractListLinked> get_list_node(QModelIndex const& index) const {
        return get_node_as<core::AbstractListLinked>(index);
    }

    size_t get_node_index(QModelIndex const& index) const;

    core::NodeTree::Index get_inner_index(QModelIndex const& parent, size_t i) const;
    core::NodeTree::Index get_inner_index(QModelIndex const& index) const;

    QModelIndex from_inner_index(core::NodeTree::Index index) const;

private:
    core::AbstractReference root;
    shared_ptr<core::NodeTree> tree;
    shared_ptr<core::ActionStack> action_stack;

    mutable map<pair<QModelIndex, size_t>, quintptr> indexes;
    mutable map<quintptr, QModelIndex> parents;
    mutable quintptr index_count = 0;
};

} // namespace rainynite::studio

#endif
