/*
 *  time_item.h - time editing item
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

#ifndef __STUDIO__CANVAS__TIME_ITEM_H__DD92BED4
#define __STUDIO__CANVAS__TIME_ITEM_H__DD92BED4

#include <QGraphicsRectItem>
#include <QGuiApplication>

namespace studio {

class TimeItem : public QGraphicsRectItem {
public:
    using Callback = std::function<void(core::Time)>;
public:
    TimeItem(Callback callback_) :
        QGraphicsRectItem {0, 0, 2, 80},
        callback(callback_)
    {
        setBrush(QGuiApplication::palette().text());
        setPen({Qt::NoPen});
    }
public:
    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override {
        if (change == ItemPositionChange) {
            auto pos = value.toPointF();
            auto new_x = change_pos(pos.x());
            return QGraphicsItem::itemChange(change, QPointF{ new_x, y() });
        }
        return QGraphicsItem::itemChange(change, value);
    }
public:
    void move_to(core::Time time) {
        auto s = time.get_seconds();
        setPos(s*x_zoom_factor, 0);
    }
    void set_fps(core::Time::fps_type fps_) {
        fps = fps_;
    }
    void set_readonly(bool ro) {
        setFlag(QGraphicsItem::ItemIsMovable, !ro);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
    }
    void set_pos_height(int y, int height) {
        auto r = rect();
        r.setY(y);
        r.setHeight(height);
        setRect(r);
    }
private:
    double change_pos(double x) {
        int frames = x * fps / x_zoom_factor;
        callback(core::Time(0, fps, frames));
        return (double)frames * x_zoom_factor / fps;
    }
private:
    Callback callback;
    core::Time::fps_type fps = 1;
    const double x_zoom_factor = 16;
};

} // namespace studio

#endif
