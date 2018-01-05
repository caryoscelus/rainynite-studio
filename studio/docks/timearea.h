/*  docks/timearea.h - time dock
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

#ifndef STUDIO_DOCKS_TIMEAREA_H_2F4F72BD_FE7C_50B3_BA68_26B2F94D4AD6
#define STUDIO_DOCKS_TIMEAREA_H_2F4F72BD_FE7C_50B3_BA68_26B2F94D4AD6

#include <core/std/memory.h>

#include <core/context.h>

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class TimeareaDock;
}

namespace rainynite::studio {

class NodeListModel;

/**
 * Time area dock
 */
class TimeareaDock : public DockWidget, public ContextListener {
public:
    explicit TimeareaDock(shared_ptr<EditorContext> context_, QWidget* parent = nullptr);
    virtual ~TimeareaDock();

    bool eventFilter(QObject* object, QEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void set_context(shared_ptr<EditorContext> context) override;

protected:
    void reload_editors();
    void add_editors();
    void remove_editors(QModelIndex const& parent, int first, int last);

    void update_ruler();
    void load_pinned_from_file(shared_ptr<core::AbstractDocument> new_document);

private:
    vector<vector<shared_ptr<AbstractCanvasEditor>>> editor_list;
    unique_ptr<Ui::TimeareaDock> ui;
    unique_ptr<NodeListModel> node_list_model;
    weak_ptr<core::AbstractDocument> document;
    bool pinned = false;
    size_t editor_count = 0;
};

REGISTER_DOCK("Timeline", TimeareaDock, Qt::BottomDockWidgetArea);

} // namespace rainynite::studio

#endif
