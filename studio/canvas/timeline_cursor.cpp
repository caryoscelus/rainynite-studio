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

class TimelineCursor;

class CursorItem : public QGraphicsRectItem {
public:
    CursorItem(TimelineCursor* parent_) :
        QGraphicsRectItem {0, 0, 4, 80},
        parent(parent_)
    {
    }
public:
    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override {
        if (change == ItemPositionChange) {
            auto pos = value.toPointF();
            auto new_x = change_pos(pos.x());
            return QGraphicsItem::itemChange(change, QPointF{ new_x, 0 });
        }
        return QGraphicsItem::itemChange(change, value);
    }
public:
    void move_to(core::Time time) {
        auto s = time.get_seconds();
        setPos(s*x_zoom_factor, 0);
    }
    double change_pos(double x);
private:
    TimelineCursor* parent;
    const double x_zoom_factor = 16;
};

class TimelineCursor : public TimelineEditor, public ContextListener {
public:
    void set_canvas(TimelineArea* canvas) override {
        TimelineEditor::set_canvas(canvas);
        cursor_item = std::make_unique<CursorItem>(this);
        canvas->scene()->addItem(cursor_item.get());
        cursor_item->setFlag(QGraphicsItem::ItemIsMovable, true);
        cursor_item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        cursor_item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
public:
    double set_time(double seconds) {
        auto fps = get_core_context()->get_fps();
        int frames = seconds*fps;
        ignore_time_change = true;
        get_core_context()->set_frames(frames);
        ignore_time_change = false;
        return (double)frames / fps;
    }
public:
    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        if (cursor_item && !ignore_time_change)
            cursor_item->move_to(time);
    }
private:
    std::unique_ptr<CursorItem> cursor_item;
    bool ignore_time_change = false;
};

double CursorItem::change_pos(double x) {
    return parent->set_time(x/x_zoom_factor) * x_zoom_factor;
}

REGISTER_TIMELINE_EDITOR(TimelineCursor, TimelineCursor);

}
