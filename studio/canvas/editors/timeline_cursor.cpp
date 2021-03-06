/*  timeline_cursor.cpp - timeline cursor widget
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

#include <QGraphicsRectItem>

#include <generic/context_listener.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include "time_item.h"

namespace rainynite::studio {

class TimelineCursor;

class CursorItem : public TimeItem {
public:
    CursorItem(TimelineCursor* parent) :
        p(parent)
    {
        set_readonly(false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* /*event*/) override {
        if (auto canvas = scene()) {
            canvas->clearSelection();
        }
    }

protected:
    void position_changed(core::Time time) override;

private:
    observer_ptr<TimelineCursor> p;
};

/**
 * Cursor for timeline area.
 *
 * Uses TimeItem (through CursorItem) for real work.
 * TODO: merge generic parts with TimeEditor
 */
class TimelineCursor :
    public TimelineEditor,
    public ContextListener
{
public:
    void setup_canvas() override {
        time_item = make_unique<CursorItem>(this);
        get_scene()->addItem(time_item.get());
        time_item->set_pos_height(0, 1024); // "infinitely" big
    }

    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        if (time_item && !ignore_time_change)
            time_item->move_to(time);
    }
    void fps_changed(core::Time::fps_type fps) override {
        ContextListener::fps_changed(fps);
        if (time_item)
            time_item->set_fps(fps);
    }

    void cursor_moved(core::Time time) {
        ignore_time_change = true;
        get_core_context()->set_time(time);
        ignore_time_change = false;
    }

private:
    unique_ptr<CursorItem> time_item;
    bool ignore_time_change = false;
};

void CursorItem::position_changed(core::Time time) {
    p->cursor_moved(time);
}

REGISTER_CANVAS_EDITOR_NAME(TimelineArea, TimelineCursor, TimelineCursor);

} // rainynite::studio
