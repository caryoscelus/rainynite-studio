/*
 *  timeline_cursor.cpp - timeline cursor widget
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

#include <QGraphicsRectItem>

#include <generic/context_listener.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>

namespace studio {

class TimelineCursor : public TimelineEditor, public ContextListener {
public:
    void set_canvas(TimelineArea* canvas) override {
        TimelineEditor::set_canvas(canvas);
        cursor_item.reset(canvas->scene()->addRect({0, 0, 4, 80}));
        cursor_item->setFlag(QGraphicsItem::ItemIsMovable, true);
        cursor_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        cursor_item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
public:
    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        move_cursor(time);
    }
private:
    void move_cursor(core::Time time) {
        if (!cursor_item)
            return;
        auto s = time.get_seconds();
        cursor_item->setPos(s*x_zoom_factor, 0);
    }
private:
    std::unique_ptr<QGraphicsRectItem> cursor_item;
    const double x_zoom_factor = 16;
};

REGISTER_TIMELINE_EDITOR(TimelineCursor, TimelineCursor);

}
