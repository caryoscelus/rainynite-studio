/*  docks/node_edit.h - widget-based node editing dock
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

#ifndef STUDIO_DOCKS_NODE_EDIT_DOCK_H_633C5258_EC08_508A_AB1E_77E71F094702
#define STUDIO_DOCKS_NODE_EDIT_DOCK_H_633C5258_EC08_508A_AB1E_77E71F094702

#include <core/std/memory.h>
#include <core/context.h>

#include <generic/node_editor.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class NodeEditDock;
}

namespace rainynite::studio {

/**
 * Node editing dock, supporting basic text editor and custom editors based on
 * node type.
 */
class NodeEditDock : public DockWidget, public NodeEditor {
public:
    explicit NodeEditDock(shared_ptr<EditorContext> context_, QWidget* parent = nullptr);
    ~NodeEditDock();

    void active_node_index_changed(core::NodeTreeIndex index) override;
    void time_changed(core::Time time) override;

private:
    void update_value();
    void update_generic();
    void update_custom();
    void setup_custom_widget(shared_ptr<core::AbstractValue> node);

private:
    unique_ptr<Ui::NodeEditDock> ui;
    unique_ptr<QWidget> custom_widget;

    core::NodeTreeIndex active_node;
};

REGISTER_CONTEXT_DOCK("Node editor", NodeEditDock, Qt::LeftDockWidgetArea);

} // namespace rainynite::studio

#endif
