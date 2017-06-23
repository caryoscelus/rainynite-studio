/*
 *  node_edit_dock.h - simple text editing of selecting node
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

#ifndef __STUDIO__NODE_EDIT_DOCK_H__49AA85D4
#define __STUDIO__NODE_EDIT_DOCK_H__49AA85D4

#include <memory>

#include <core/context.h>

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class NodeEditDock;
}

namespace studio {

/**
 * Node editing dock, supporting basic text editor and custom editors based on
 * node type.
 */
class NodeEditDock : public DockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit NodeEditDock(std::shared_ptr<EditorContext> context_, QWidget* parent = nullptr);
    ~NodeEditDock();

public:
    virtual void active_node_changed(std::shared_ptr<core::AbstractValue> node) override;
    virtual void time_changed(core::Time time) override;

private Q_SLOTS:
    void write_node();

private:
    void update_value();
    void update_generic();
    void update_custom();
    void setup_custom_widget(std::shared_ptr<core::AbstractValue> node);

private:
    std::unique_ptr<Ui::NodeEditDock> ui;
    QWidget* custom_widget = nullptr;

    std::shared_ptr<core::AbstractValue> active_node;
};

REGISTER_DOCK("Node editor", NodeEditDock, Qt::LeftDockWidgetArea);

}

#endif
