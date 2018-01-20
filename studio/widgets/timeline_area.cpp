/*  timeline_area.cpp - advanced timeline
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

#include <QGraphicsScene>
#include <QContextMenuEvent>
#include <QMenu>

#include <generic/timeline_editor.h>

#include "timeline_area.h"

namespace rainynite::studio {

TimelineArea::TimelineArea(QWidget* parent) :
    AbstractCanvas(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setSceneRect(-5, 0, 65, 30);
    zoom_time_by(24);
}

TimelineArea::~TimelineArea() {
}

void TimelineArea::contextMenuEvent(QContextMenuEvent* event) {
    if (event->type() == QEvent::ContextMenu) {
        auto cm_event = static_cast<QContextMenuEvent*>(event);
        auto pos = mapToScene(cm_event->pos());
        // TODO: find editor at this y
        for (auto editor : list_editors()) {
            QMenu menu;
            if (auto timeline_editor = dynamic_cast<TimelineEditor*>(editor.get())) {
                if (timeline_editor->call_context_menu(pos.y(), pos.x(), menu)) {
                    menu.popup(event->globalPos());
                    event->accept();
                }
            }
        }
    }
    return AbstractCanvas::contextMenuEvent(event);
}

void TimelineArea::zoom_time_by(double factor) {
    scale(factor, 1.0);
    Q_EMIT zoomed();
}

void TimelineArea::add_misc_editor(shared_ptr<AbstractCanvasEditor> editor) {
    misc_editors.push_back(editor);
}

} // namespace rainynite::studio
