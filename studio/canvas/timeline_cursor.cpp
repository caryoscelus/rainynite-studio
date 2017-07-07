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
#include "time_item.h"

namespace studio {

/**
 * Cursor for timeline area.
 *
 * Uses TimeItem for real work.
 * TODO: merge generic parts with TimeEditor
 */
class TimelineCursor :
    public TimelineEditor,
    public ContextListener,
    public AbstractTimeEditor
{
public:
    void set_canvas(TimelineArea* canvas) override {
        TimelineEditor::set_canvas(canvas);
        time_item = std::make_unique<TimeItem>(this);
        canvas->scene()->addItem(time_item.get());
        time_item->set_readonly(false);
        time_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }
public:
    void set_time(core::Time time) override {
        ignore_time_change = true;
        get_core_context()->set_time(time);
        ignore_time_change = false;
    }
public:
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
private:
    std::unique_ptr<TimeItem> time_item;
    bool ignore_time_change = false;
};

REGISTER_TIMELINE_EDITOR(TimelineCursor, TimelineCursor);

}
