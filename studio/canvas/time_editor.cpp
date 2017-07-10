/*
 *  time_editor.cpp - time editing widget
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

#include <generic/node_editor.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include "time_item.h"

namespace studio {

class TimeEditor :
    public TimelineEditor,
    public AbstractTimeEditor,
    public NodeEditor
{
public:
    void set_canvas(TimelineArea* canvas) override {
        TimelineEditor::set_canvas(canvas);
        time_item = std::make_unique<TimeItem>(this);
        canvas->scene()->addItem(time_item.get());
        node_update();
    }
public:
    void set_time(core::Time time) override {
        if (auto node = get_node_as<core::Time>()) {
            ignore_time_change = true;
            if (node->can_set())
                node->set(time);
            ignore_time_change = false;
        }
    }
public:
    void node_update() override {
        update_position();
        auto node = get_node_as<core::Time>();
        bool editable = node && node->can_set();
        time_item->set_readonly(!editable);
        time_item->setFlag(QGraphicsItem::ItemIsSelectable, editable);
    }
    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        update_position();
    }
    void fps_changed(core::Time::fps_type fps) override {
        ContextListener::fps_changed(fps);
        if (time_item)
            time_item->set_fps(fps);
    }
private:
    void update_position() {
        if (time_item && !ignore_time_change) {
            if (auto maybe_time = get_value<core::Time>())
                time_item->move_to(*maybe_time);
        }
    }
private:
    std::unique_ptr<TimeItem> time_item;
    bool ignore_time_change = false;
};

REGISTER_TIMELINE_NODE_EDITOR(TimeEditor, core::Time, TimeEditor);

}
