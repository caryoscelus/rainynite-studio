/*  time_editor.cpp - time editing widget
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

#include <core/action_stack.h>
#include <core/actions/change_value.h>

#include <generic/node_editor.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include "time_item.h"

namespace rainynite::studio {

class TimeEditor;

class TimeEditorItem : public TimeItem {
public:
    TimeEditorItem(TimeEditor* parent) :
        p(parent)
    {}
protected:
    void position_changed(core::Time time) override;
    void selected_changed(bool is_selected) override;
private:
    observer_ptr<TimeEditor> p;
};

class TimeEditor :
    public TimelineEditor,
    public NodeEditor
{
public:
    void setup_canvas() override {
        time_item = make_unique<TimeEditorItem>(this);
        get_scene()->addItem(time_item.get());
        node_update();
    }
    void set_position_hint(int y, int height) override {
        time_item->set_pos_height(y, height);
    }

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

    void moved(core::Time time) {
        if (auto node = get_node_as<core::Time>()) {
            ignore_time_change = true;
            if (node->can_set()) {
                if (auto action_stack = get_context()->action_stack()) {
                    using core::actions::ChangeValue;
                    action_stack->emplace<ChangeValue>(node, time);
                }
            }
            ignore_time_change = false;
        }
    }

private:
    void update_position() {
        if (time_item && !ignore_time_change) {
            if (auto maybe_time = get_value<core::Time>()) {
                auto time = *maybe_time;
                time_item->set_fps(time.get_fps());
                time_item->move_to(time);
            }
        }
    }
private:
    unique_ptr<TimeItem> time_item;
    bool ignore_time_change = false;
};

void TimeEditorItem::position_changed(core::Time time) {
    p->moved(time);
}

void TimeEditorItem::selected_changed(bool is_selected) {
    if (is_selected)
        p->activate_node();
}

REGISTER_CANVAS_EDITOR(TimelineArea, TimeEditor, core::Time);

}
