/*  docks/timearea.cpp - time area / timeline dock
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
#include <QScrollBar>

#include <core/node/abstract_value.h>
#include <core/node/abstract_node.h>
#include <core/node/traverse.h>
#include <core/node/make.h>
#include <core/document.h>

#include <widgets/timeline_area.h>
#include <generic/timeline_editor.h>
#include <models/node_list.h>
#include "timearea.h"
#include "ui_timearea.h"

namespace rainynite::studio {

TimeareaDock::TimeareaDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(make_unique<Ui::TimeareaDock>()),
    node_list_model(make_unique<NodeListModel>())
{
    node_list_model->set_context(get_context());
    ui->setupUi(this);
    auto cursor = add_canvas_named_editor(*ui->timeline, "TimelineCursor");
    ui->timeline->add_misc_editor(cursor);
    ui->timeline->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->timeline->load_registered_tools();
    ui->timeline->use_tool("Default");
    ui->node_list->setModel(node_list_model.get());

    connect(node_list_model.get(), &QAbstractItemModel::layoutChanged, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsInserted, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsMoved, this, &TimeareaDock::update_editors);
    connect(node_list_model.get(), &QAbstractItemModel::rowsRemoved, this, &TimeareaDock::update_editors);

    ui->timeline->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->node_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(ui->timeline->verticalScrollBar(), &QScrollBar::valueChanged, ui->node_list->verticalScrollBar(), &QScrollBar::setValue);
    connect(ui->node_list->verticalScrollBar(), &QScrollBar::valueChanged, ui->timeline->verticalScrollBar(), &QScrollBar::setValue);

    connect(ui->timeline->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TimeareaDock::update_ruler);
    connect(ui->timeline->scene(), &QGraphicsScene::sceneRectChanged, this, &TimeareaDock::update_ruler);
    connect(ui->timeline, &AbstractCanvas::zoomed, this, &TimeareaDock::update_ruler);
    ui->timeline->installEventFilter(this);
    update_ruler();

    set_context(get_context());
}

TimeareaDock::~TimeareaDock() {
}

bool TimeareaDock::eventFilter(QObject* object, QEvent* event) {
    if (object == ui->timeline && event->type() == QEvent::Resize)
        update_ruler();
    return false;
}

void TimeareaDock::contextMenuEvent(QContextMenuEvent* event) {
    auto index = ui->node_list->selectionModel()->currentIndex();
    if (index.isValid()) {
        QMenu menu(this);
        if (index.row() == node_list_model->rowCount()-1 && !pinned) {
            menu.addAction(
                QIcon::fromTheme("list-add"),
                "Pin to timeline area",
                [this, index]() {
                    pinned = true;
                    // TODO: check for existing property, etc
                    if (auto node = abstract_node_cast(node_list_model->get_node(index)))
                        node->set_property("_pin_in_timeline", core::make_value<bool>(true));
                }
            );
        } else {
            menu.addAction(
                QIcon::fromTheme("list-remove"),
                "Remove from timeline area",
                [this, index]() {
                    node_list_model->removeRow(index.row());
                    if (auto node = abstract_node_cast(node_list_model->get_node(index)))
                        node->remove_property("_pin_in_timeline");
                }
            );
        }
        menu.exec(event->globalPos());
    }
}

void TimeareaDock::set_context(shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    ui->timeline->set_context(context);
    ui->ruler->set_context(context);
    node_list_model->set_context(context);
    if (auto new_document = context->get_context()->get_document()) {
        document = new_document;
        load_pinned_from_file(std::move(new_document));
    }
    connect_boost(
        context->changed_active_node(),
        [this](auto index) {
            if (!pinned && node_list_model->rowCount() > 0)
                node_list_model->removeRow(node_list_model->rowCount()-1);
            pinned = !node_list_model->insert_unique_node(index);
        }
    );
}

void TimeareaDock::load_pinned_from_file(shared_ptr<core::Document> new_document) {
//     core::traverse_once<bool>(
//         new_document,
//         [this](core::AbstractReference value) -> optional<bool> {
//             // TODO: support dynamic _pin_in_timeline
//             if (auto node = abstract_node_cast(value)) {
//                 if (node->get_property_value<bool>("_pin_in_timeline", get_core_context()).value_or(false))
//                     node_list_model->insert_unique_node(value);
//             }
//             return {};
//         }
//     );
}

void TimeareaDock::update_editors() {
    ui->timeline->clear_editors();
    for (int i = 0; i < node_list_model->rowCount(); ++i) {
        auto index = node_list_model->index(i, 0);
        auto inner_index = node_list_model->get_inner_index(index);
        auto editors = add_canvas_node_editor(*ui->timeline, inner_index);
        for (auto editor : editors) {
            auto rect = ui->node_list->visualRect(index);
            if (auto timeline_editor = dynamic_cast<TimelineEditor*>(editor.get()))
                timeline_editor->set_position_hint(rect.y(), rect.height());
        }
    }
}

void TimeareaDock::update_ruler() {
    if (ui == nullptr)
        return;
    auto timeline_x = ui->timeline->mapFromScene(0, 0);
    auto global_x = ui->timeline->mapToGlobal(timeline_x);
    // no idea why that +4 offset is necessary..
    ui->ruler->set_scroll(ui->ruler->mapFromGlobal(global_x).x()+4);
    ui->ruler->set_zoom(ui->timeline->transform().m11());
}

} // namespace rainynite::studio
