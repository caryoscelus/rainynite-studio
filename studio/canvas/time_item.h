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

#include <functional>

#include <QGraphicsRectItem>

#include <core/time/time.h>

namespace studio {

class TimeItem : public QGraphicsRectItem {
public:
    using Callback = std::function<void(core::Time)>;
public:
    TimeItem(Callback callback_);
public:
    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
public:
    void move_to(core::Time time);
    void set_fps(core::Time::fps_type fps_) {
        fps = fps_;
    }
    void set_readonly(bool ro);
    void set_pos_height(int y, int height);
private:
    double change_pos(double x);
private:
    Callback callback;
    core::Time::fps_type fps = 1;
    const double x_zoom_factor = 16;
};

} // namespace studio

#endif
