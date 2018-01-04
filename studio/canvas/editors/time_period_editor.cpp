/*  time_period_editor.cpp - time period editing widget
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

class TimePeriodEditor;

class TimePeriodItem : public TimeItem {
public:
    using Setter = void (core::TimePeriod::*)(core::Time);

    TimePeriodItem(TimePeriodEditor* parent, Setter setter_) :
        p(parent),
        setter(setter_)
    {}

protected:
    void position_changed(core::Time time) override;

private:
    observer_ptr<TimePeriodEditor> p;
    Setter setter;
};

class TimePeriodEditor :
    public TimelineEditor,
    public NodeEditor
{
public:
    void setup_canvas() override {
        first_item = make_unique<TimePeriodItem>(this, &core::TimePeriod::set_first);
        last_item = make_unique<TimePeriodItem>(this, &core::TimePeriod::set_last);
        get_scene()->addItem(first_item.get());
        get_scene()->addItem(last_item.get());
        node_update();
    }
    void set_position_hint(int y, int height) override {
        first_item->set_pos_height(y, height);
        last_item->set_pos_height(y, height);
    }

    void node_update() override {
        update_position();
        auto node = get_node_as<core::TimePeriod>();
        bool editable = node && node->can_set();
        for (auto item : {first_item.get(), last_item.get()}) {
            item->set_readonly(!editable);
            item->setFlag(QGraphicsItem::ItemIsSelectable, editable);
        }
    }
    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        update_position();
    }

    void item_moved(core::Time time, TimePeriodItem::Setter setter) {
        if (auto node = get_node_as<core::TimePeriod>()) {
            ignore_time_change = true;
            if (node->can_set()) {
                if (auto action_stack = get_context()->action_stack()) {
                    using core::actions::ChangeValue;
                    auto value = node->mod();
                    (value.*setter)(time);
                    action_stack->emplace<ChangeValue>(node, value);
                }
            }
            ignore_time_change = false;
        }
    }

private:
    void update_position() {
        if (first_item && !ignore_time_change) {
            if (auto maybe_value = get_value<core::TimePeriod>()) {
                auto value = *maybe_value;
                first_item->set_fps(value.get_fps());
                first_item->move_to(value.get_first());
                last_item->set_fps(value.get_fps());
                last_item->move_to(value.get_last());
            }
        }
    }
private:
    unique_ptr<TimePeriodItem> first_item;
    unique_ptr<TimePeriodItem> last_item;
    bool ignore_time_change = false;
};

void TimePeriodItem::position_changed(core::Time time) {
    p->item_moved(time, setter);
}

REGISTER_CANVAS_EDITOR(TimelineArea, TimePeriodEditor, core::TimePeriod);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(CompositeTimePeriod, true);

}
