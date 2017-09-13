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

#include <algorithm>

#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>

#include <core/document.h>
#include <core/node_info.h>

#include <util/strings.h>
#include <models/node_model.h>
#include <generic/node_menu.h>
#include "node_tree_dock.h"
#include "ui_node_tree_dock.h"

namespace rainynite::studio {

NodeTreeDock::NodeTreeDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(make_unique<Ui::NodeTreeDock>())
{
    ui->setupUi(this);
    ui->tree_view->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    connect(ui->tree_view, SIGNAL(activated(QModelIndex)), this, SLOT(activate(QModelIndex)));
    set_context(get_context());
}

NodeTreeDock::~NodeTreeDock() {
}

void NodeTreeDock::set_context(shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    if (auto document = get_core_context()->get_document()) {
        model = make_unique<NodeModel>(document, document->get_action_stack());
    } else {
        model = make_unique<NodeModel>(nullptr, nullptr);
    }
    model->set_context(context_);
    ui->tree_view->setModel(model.get());
}

void NodeTreeDock::contextMenuEvent(QContextMenuEvent* event) {
    auto selection_model = ui->tree_view->selectionModel();
    if (menu = node_context_menu(model.get(), selection_model, get_time())) {
        menu->exec(event->globalPos());
    }
}

void NodeTreeDock::activate(QModelIndex const& index) {
    auto node = model->get_node(index);
    Q_EMIT activated(node);
}

} // namespace rainynite::studio
