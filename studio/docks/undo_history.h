/*  docks/undo_history.h - undo/redo history dock
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

#ifndef STUDIO_DOCKS_UNDO_HISTORY_H_4A9B9622_73CA_5E46_889F_7912CD326698
#define STUDIO_DOCKS_UNDO_HISTORY_H_4A9B9622_73CA_5E46_889F_7912CD326698

#include <core/std/memory.h>

#include <generic/dock_registry.h>
#include <generic/context_listener.h>
#include "close_destroy_dock.h"

namespace Ui {
class UndoHistoryDock;
}

namespace rainynite::studio {

class UndoModel;

class UndoHistoryDock : public DockWidget, public ContextListener {
public:
    UndoHistoryDock(shared_ptr<EditorContext> context, QWidget* parent=nullptr);
    virtual ~UndoHistoryDock();

    void set_context(shared_ptr<EditorContext> context) override;

private:
    unique_ptr<Ui::UndoHistoryDock> ui;
    unique_ptr<UndoModel> model;
};

REGISTER_DOCK("Undo history", UndoHistoryDock, Qt::RightDockWidgetArea);

} // namespace rainynite::studio

#endif
