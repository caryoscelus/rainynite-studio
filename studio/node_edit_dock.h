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

#include <QDockWidget>

#include <core/context.h>

#include "context_listener.h"

namespace Ui {
class NodeEditDock;
}

namespace studio {

/**
 * Dock for simple text node editing
 */
class NodeEditDock : public QDockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit NodeEditDock(std::shared_ptr<core::Context> context_, QWidget* parent = 0);
    ~NodeEditDock();

public:
    virtual void active_node_changed(std::shared_ptr<core::AbstractValue> node) override;

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private Q_SLOTS:
    void write_node();

private:
    std::unique_ptr<Ui::NodeEditDock> ui;
    std::shared_ptr<core::AbstractValue> active_node;
};

}

#endif
