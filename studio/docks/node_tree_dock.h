/*
 *  node_tree_dock.h - Dock with node tree
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

#ifndef __STUDIO__NODE_TREE_DOCK_H__8AC1EE40
#define __STUDIO__NODE_TREE_DOCK_H__8AC1EE40

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace core {
class AbstractValue;
}

namespace Ui {
class NodeTreeDock;
}

class QMenu;

namespace studio {

class NodeModel;

/**
 * Node graph tree representation dock.
 *
 * Until visual node editor is done, this is the main place for node
 * manipulation.
 */
class NodeTreeDock : public DockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit NodeTreeDock(std::shared_ptr<EditorContext> context_, QWidget* parent_ = nullptr);
    virtual ~NodeTreeDock();

public:
    void set_context(std::shared_ptr<EditorContext> context_) override;

Q_SIGNALS:
    void activated(std::shared_ptr<core::AbstractValue> node);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private Q_SLOTS:
    void activate(QModelIndex const& index);

private:
    std::unique_ptr<Ui::NodeTreeDock> ui;
    std::unique_ptr<NodeModel> model;
    std::unique_ptr<QMenu> menu;
};

REGISTER_DOCK("Node tree", NodeTreeDock, Qt::LeftDockWidgetArea);

} // namespace studio

#endif
