/*
 *  node_tree_dock.cpp - Dock with node tree
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

#include <core/document.h>

#include "node_model.h"
#include "node_tree_dock.h"
#include "ui_node_tree_dock.h"

namespace studio {

NodeTreeDock::NodeTreeDock(std::shared_ptr<core::Context> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::NodeTreeDock>())
{
    ui->setupUi(this);
    connect(ui->tree_view, SIGNAL(activated(QModelIndex)), this, SLOT(activate(QModelIndex)));
    set_context(get_context());
}

void NodeTreeDock::set_context(std::shared_ptr<core::Context> context_) {
    ContextListener::set_context(context_);
    if (auto document = context_->get_document()) {
        model = std::make_unique<NodeModel>(document->get_root());
    } else {
        model = std::make_unique<NodeModel>(nullptr);
    }
    ui->tree_view->setModel(model.get());
}

NodeTreeDock::~NodeTreeDock() {
}

void NodeTreeDock::activate(QModelIndex const& index) {
    auto node = dynamic_cast<NodeModel*>(ui->tree_view->model())->get_node(index);
    Q_EMIT activated(node);
}

} // namespace studio
