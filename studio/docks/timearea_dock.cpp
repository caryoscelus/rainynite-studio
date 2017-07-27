/*
 *  timearea_dock.cpp - time area / timeline dock
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

#include <core/node/abstract_value.h>

#include <widgets/timeline_area.h>
#include <generic/timeline_editor.h>
#include <models/node_list.h>
#include "timearea_dock.h"
#include "ui_timearea_dock.h"

namespace studio {

TimeareaDock::TimeareaDock(std::shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::TimeareaDock>()),
    node_list_model(std::make_unique<NodeListModel>())
{
    ui->setupUi(this);
    add_timeline_named_editor(*ui->timeline, "TimelineCursor");
    ui->node_list->setModel(node_list_model.get());

    connect(node_list_model.get(), &QAbstractItemModel::layoutChanged, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsInserted, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsMoved, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsRemoved, this, &TimeareaDock::update_editors);

    set_context(get_context());
}

TimeareaDock::~TimeareaDock() {
}

void TimeareaDock::contextMenuEvent(QContextMenuEvent* event) {
    auto index = ui->node_list->selectionModel()->currentIndex();
    if (index.isValid()) {
        QMenu menu(this);
        menu.addAction(
            QIcon::fromTheme("list-remove"),
            "Remove from timeline area",
            [this, index]() {
                node_list_model->removeRow(index.row());
            }
        );
        menu.exec(event->globalPos());
    }
}

void TimeareaDock::set_context(std::shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    ui->timeline->set_context(context);
    connect_boost(
        context->changed_active_node(),
        [this](core::AbstractReference node) {
            node_list_model->insert_unique_node(node);
        }
    );
}

void TimeareaDock::update_editors() {
    ui->timeline->clear_node_editors();
    for (auto node : node_list_model->get_all_nodes()) {
        add_timeline_node_editor(*ui->timeline, node);
    }
}

} // namespace studio
