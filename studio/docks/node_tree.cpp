/*  docks/node_tree.cpp - Dock with node tree
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

#include <algorithm>

#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>

#include <core/document.h>
#include <core/node_info/node_info.h>
#include <core/util/nullptr.h>

#include <util/strings.h>
#include <models/node_model.h>
#include <models/model_filter.h>
#include <generic/node_menu.h>
#include "node_tree.h"
#include "ui_node_tree.h"

namespace rainynite::studio {

NodeTreeDock::NodeTreeDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(make_unique<Ui::NodeTreeDock>())
{
    ui->setupUi(this);
    ui->tree_view->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

    // Unfortunately, Qt mimics platform-dependent behaviour (double click for
    // some platforms like Gtk) for `activated` signal, so we have to duplicate
    // with `clicked` in order to make sure single click activates nodes
    connect(ui->tree_view, &QAbstractItemView::activated, this, &NodeTreeDock::activate);
    connect(ui->tree_view, &QAbstractItemView::clicked, this, &NodeTreeDock::activate);

    connect(ui->filter, &QLineEdit::textEdited, this, &NodeTreeDock::apply_filter);

    connect(ui->reload_model, &QAbstractButton::clicked, this, &NodeTreeDock::reload_model);

    set_context(get_context());
}

NodeTreeDock::~NodeTreeDock() {
}

void NodeTreeDock::set_context(shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    if (auto document = get_core_context()->get_document()) {
        model = make_unique<NodeModel>(abstract_value_cast(document), document->get_action_stack());
    } else {
        model = make_unique<NodeModel>(nullptr, nullptr);
    }
    model->set_context(context_);
    proxy_model = make_unique<RecursiveFilterProxyModel>();
    proxy_model->setSourceModel(model.get());
    apply_filter(ui->filter->text());
}

void NodeTreeDock::active_node_index_changed(core::NodeTree::Index index) {
    ContextListener::active_node_index_changed(index);
    auto selection_model = ui->tree_view->selectionModel();
    auto qt_index = model->from_inner_index(index);
    selection_model->select(qt_index, QItemSelectionModel::Current);
    ui->tree_view->scrollTo(qt_index);
}

void NodeTreeDock::contextMenuEvent(QContextMenuEvent* event) {
    auto selection_model = ui->tree_view->selectionModel();
    if (menu = node_context_menu(model.get(), selection_model, get_time())) {
        menu->exec(event->globalPos());
    }
}

void NodeTreeDock::apply_filter(QString const& s) {
    if (s.isEmpty()) {
        ui->tree_view->setModel(model.get());
    } else {
        proxy_model->setFilterRegExp({s});
        ui->tree_view->setModel(proxy_model.get());
    }
}

void NodeTreeDock::activate(QModelIndex const& index) {
    set_active_node(model->get_inner_index(index));
}

void NodeTreeDock::reload_model() {
    auto tree = no_null(get_context()->tree());
    tree->reload_children(tree->get_root_index());
}

} // namespace rainynite::studio
