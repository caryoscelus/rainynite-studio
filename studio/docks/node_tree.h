/*  docks/node_tree.h - Dock with node tree
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

#ifndef STUDIO_DOCKS_NODE_TREE_DOCK_H_85472C8A_FD7F_5252_A26B_79DF8D0503A0
#define STUDIO_DOCKS_NODE_TREE_DOCK_H_85472C8A_FD7F_5252_A26B_79DF8D0503A0

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class NodeTreeDock;
}

class QMenu;

namespace rainynite::studio {

class NodeModel;
class RecursiveFilterProxyModel;

/**
 * Node graph tree representation dock.
 *
 * Until visual node editor is done, this is the main place for node
 * manipulation.
 */
class NodeTreeDock : public DockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit NodeTreeDock(shared_ptr<EditorContext> context_, QWidget* parent_ = nullptr);
    virtual ~NodeTreeDock();

public:
    void set_context(shared_ptr<EditorContext> context_) override;

    void active_node_index_changed(core::NodeTree::Index index) override;

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

    void apply_filter(QString const& s);

private:
    void activate(QModelIndex const& index);

private:
    unique_ptr<Ui::NodeTreeDock> ui;
    unique_ptr<NodeModel> model;
    unique_ptr<RecursiveFilterProxyModel> proxy_model;
    unique_ptr<QMenu> menu;
};

REGISTER_CONTEXT_DOCK("Node tree", NodeTreeDock, Qt::LeftDockWidgetArea);

} // namespace rainynite::studio

#endif
