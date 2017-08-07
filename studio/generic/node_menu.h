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

#ifndef __STUDIO__GENERIC__NODE_MENU_H__A418F828
#define __STUDIO__GENERIC__NODE_MENU_H__A418F828

#include <memory>

#include <QMenu>

class QItemSelectionModel;

namespace studio {

class NodeContextMenu : public QMenu {
public:
    NodeContextMenu(NodeModel* model, QItemSelectionModel* selection_model, core::Time time);
};

inline std::unique_ptr<QMenu> node_context_menu(NodeModel* model, QItemSelectionModel* selection_model, core::Time time) {
    return std::make_unique<NodeContextMenu>(model, selection_model, time);
}

} // namespace studio

#endif
