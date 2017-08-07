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

NodeContextMenu::NodeContextMenu(NodeModel* model_, QItemSelectionModel* selection_model, core::Time time_) :
    model(model_),
    time(time_)
{
    index = selection_model->currentIndex();
    auto parent_index = index.parent();
    if (auto parent_node = std::dynamic_pointer_cast<core::AbstractListLinked>(model->get_node(parent_index))) {
        size_t node_index = model->get_node_index(index);
        auto type = parent_node->get_link_type(node_index);
        node_infos = type ? core::node_types()[*type] : core::all_node_infos();

        selection = selection_model->selectedIndexes();
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
                [this]() {
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
                [this, a, b]() {
                    model->swap_nodes(a, b);
                }
            );
        }

        if (model->node_is_connected(index)) {
            addAction(
                QIcon::fromTheme("remove-link"),
                "Disconnect",
                [this]() {
                    model->disconnect_node(index);
                }
            );
        }

        if (model->can_remove_node(index)) {
            addAction(
                QIcon::fromTheme("list-remove"), // TODO
                "Remove",
                [this]() {
                    model->remove_node(index);
                }
            );
        }

        if (model->can_add_element(index)) {
            addAction(
                QIcon::fromTheme("list-add"),
                "Add element",
                [this]() {
                    model->add_empty_element(index);
                }
            );
            addSeparator();
        }

        if (model->can_add_custom_property(index)) {
            addAction(
                QIcon::fromTheme("list-add"), // TODO
                "Add custom property",
                [this]() {
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
                [this, node_index, parent_index]() {
                    model->removeRow(node_index, parent_index);
                }
            );
            auto up_action = addAction(
                QIcon::fromTheme("go-up"),
                "Move up",
                [this, node_index, parent_index]() {
                    model->move_up(node_index, parent_index);
                }
            );
            auto down_action = addAction(
                QIcon::fromTheme("go-down"),
                "Move down",
                [this, node_index, parent_index]() {
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
            search_widget = new QLineEdit();
            connect(this, SIGNAL(aboutToShow()), search_widget, SLOT(setFocus()));
            connect(search_widget, &QLineEdit::textEdited, this, &NodeContextMenu::update_node_list);
            search_action->setDefaultWidget(search_widget);
            addAction(search_action);
            update_node_list();
        }
    } else {
        addAction("No actions for root node!");
    }
}

void NodeContextMenu::update_node_list() {
    for (auto action : convert_actions) {
        removeAction(action);
    }
    convert_actions.clear();
    auto search_string = search_widget->text();
    bool first_action = true;
    for (auto node_info : node_infos) {
        auto name = util::str(node_info->name());
        if (name.indexOf(search_string) != -1) {
            auto action = addAction(name, [this, node_info]() {
                model->convert_node(index, node_info, time);
            });
            convert_actions.push_back(action);
            if (first_action) {
                setActiveAction(action);
                first_action = false;
            }
        }
    }
}

} // namespace studio