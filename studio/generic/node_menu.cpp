/*
 *  node_menu.cpp - node context menu
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

#include <QItemSelectionModel>
#include <QInputDialog>
#include <QWidgetAction>

#include <util/strings.h>
#include <models/node_model.h>
#include "node_menu.h"

namespace studio {

NodeContextMenu::NodeContextMenu(NodeModel* model, QItemSelectionModel* selection_model, core::Time time) {
    auto index = selection_model->currentIndex();
    auto parent_index = index.parent();
    if (auto parent_node = std::dynamic_pointer_cast<core::AbstractListLinked>(model->get_node(parent_index))) {
        size_t node_index = model->get_node_index(index);
        auto type = parent_node->get_link_type(node_index);
        auto node_infos = type ? core::node_types()[*type] : core::all_node_infos();

        auto selection = selection_model->selectedIndexes();
        // selection may contain other columns, which should be ignored
        selection.erase(std::remove_if(
            std::begin(selection),
            std::end(selection),
            [](auto const& index) {
                return index.column() != 0;
            }
        ));
        if (selection.size() > 1) {
            addAction(
                QIcon::fromTheme("insert-link"),
                "Connect",
                [model, index, selection]() {
                    model->connect_nodes(selection, index);
                }
            );
        }

        if (selection.size() == 2) {
            auto a = selection[0];
            auto b = selection[1];
            addAction(
                QIcon::fromTheme("exchange-positions"),
                "Swap",
                [model, a, b]() {
                    model->swap_nodes(a, b);
                }
            );
        }

        if (model->node_is_connected(index)) {
            addAction(
                QIcon::fromTheme("remove-link"),
                "Disconnect",
                [model, index]() {
                    model->disconnect_node(index);
                }
            );
        }

        if (model->can_remove_node(index)) {
            addAction(
                QIcon::fromTheme("list-remove"), // TODO
                "Remove",
                [model, index]() {
                    model->remove_node(index);
                }
            );
        }

        if (model->can_add_element(index)) {
            addAction(
                QIcon::fromTheme("list-add"),
                "Add element",
                [model, index]() {
                    model->add_empty_element(index);
                }
            );
            addSeparator();
        }

        if (model->can_add_custom_property(index)) {
            addAction(
                QIcon::fromTheme("list-add"), // TODO
                "Add custom property",
                [model, index]() {
                    auto text = QInputDialog::getText(
                        nullptr,
                        "Add new custom property",
                        "Property name:"
                    );
                    if (!text.isEmpty())
                        model->add_empty_custom_property(index, util::str(text));
                }
            );
            addSeparator();
        }

        if (parent_node->is_editable_list()) {
            addAction(
                QIcon::fromTheme("list-remove"),
                "Remove list item",
                [model, node_index, parent_index]() {
                    model->removeRow(node_index, parent_index);
                }
            );
            auto up_action = addAction(
                QIcon::fromTheme("go-up"),
                "Move up",
                [model, node_index, parent_index]() {
                    model->move_up(node_index, parent_index);
                }
            );
            auto down_action = addAction(
                QIcon::fromTheme("go-down"),
                "Move down",
                [model, node_index, parent_index]() {
                    model->move_down(node_index, parent_index);
                }
            );
            up_action->setEnabled(model->can_move_up(node_index, parent_index));
            down_action->setEnabled(model->can_move_down(node_index, parent_index));
            addSeparator();
        }

        if (node_infos.size() == 0)
            addAction("No node types available!");
        else {
            auto search_action = new QWidgetAction(this);
            auto search_widget = new QLineEdit();
            QObject::connect(this, SIGNAL(aboutToShow()), search_widget, SLOT(setFocus()));
            search_action->setDefaultWidget(search_widget);
            addAction(search_action);
            for (auto node_info : node_infos) {
                auto name = util::str(node_info->name());
                addAction(name, [model, index, node_info, time]() {
                    model->convert_node(index, node_info, time);
                });
            }
        }
    } else {
        addAction("No actions for root node!");
    }
}

} // namespace studio
