/*
 *  node_menu.h - node context menu
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

#ifndef STUDIO_GENERIC_NODE_MENU_H_0B8A2DD1_AED8_52B2_AF1C_E607DC12CB26
#define STUDIO_GENERIC_NODE_MENU_H_0B8A2DD1_AED8_52B2_AF1C_E607DC12CB26

#include <core/std/memory.h>
#include <core/std/set.h>

#include <QMenu>

class QItemSelectionModel;
class QLineEdit;

namespace rainynite::core {
class NodeInfo;
class Time;
}

namespace rainynite::studio {

class NodeContextMenu : public QMenu {
public:
    NodeContextMenu(NodeModel* model, QItemSelectionModel* selection_model, core::Time time);
private:
    void update_node_list();
    void add_custom_property();
private:
    vector<QAction*> convert_actions;
    NodeModel* model;
    QModelIndex index;
    QList<QModelIndex> selection;
    set<core::NodeInfo const*> node_infos;
    core::Time time;
    QLineEdit* search_widget = nullptr;
};

inline unique_ptr<QMenu> node_context_menu(NodeModel* model, QItemSelectionModel* selection_model, core::Time time) {
    return make_unique<NodeContextMenu>(model, selection_model, time);
}

} // namespace rainynite::studio

#endif
