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

#include <QMenu>
#include <QContextMenuEvent>

#include <core/document.h>
#include <core/node_info.h>

#include <models/node_model.h>
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

NodeTreeDock::~NodeTreeDock() {
}

void NodeTreeDock::set_context(std::shared_ptr<core::Context> context_) {
    ContextListener::set_context(context_);
    if (auto document = context_->get_document()) {
        model = std::make_unique<NodeModel>(document);
    } else {
        model = std::make_unique<NodeModel>(nullptr);
    }
    ui->tree_view->setModel(model.get());
}

void NodeTreeDock::contextMenuEvent(QContextMenuEvent* event)
{
    auto coord = ui->tree_view->mapFromGlobal(event->globalPos());
    auto index = ui->tree_view->indexAt(coord);
    auto parent_index = index.parent();
    if (auto parent_node = std::dynamic_pointer_cast<core::AbstractListLinked>(model->get_node(parent_index))) {
        QMenu menu(this);
        auto node = model->get_node(index);
        size_t node_index = model->get_node_index(index);
        auto type = parent_node->get_link_type(node_index);
        auto node_infos = core::node_types()[type];

        if (auto list_node = dynamic_cast<core::AbstractListLinked*>(node.get())) {
            menu.addAction(
                QIcon::fromTheme("list-add"),
                "Add element",
                [list_node]() {
                    list_node->push_new();
                }
            );
            menu.addSeparator();
        }

        if (node_infos.size() == 0)
            menu.addAction("No node types available!");
        else for (auto node_info : node_infos) {
            auto name = QString::fromStdString((*node_info)());
            menu.addAction(name, [this, node_info, node, parent_node, node_index]() {
                boost::any value;
                if (node)
                    value = node->get_any(get_time());
                auto new_node = core::make_node_with_name<core::AbstractValue>(node_info->name(), value); 
                parent_node->set_link(node_index, new_node);
            });
        }
        menu.exec(event->globalPos());
    }
}

void NodeTreeDock::activate(QModelIndex const& index) {
    auto node = model->get_node(index);
    Q_EMIT activated(node);
}

} // namespace studio
