/*  time_item.h - time editing item
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

#ifndef STUDIO_CANVAS_EDITORS_TIME_ITEM_H_E6E1B74E_13BE_5F5F_B46E_771E4C2E09A3
#define STUDIO_CANVAS_EDITORS_TIME_ITEM_H_E6E1B74E_13BE_5F5F_B46E_771E4C2E09A3

#include <QGraphicsRectItem>

#include <core/time/time.h>

namespace rainynite::studio {

/**
 * Time-editing canvas item that can be used to make editors on timeline.
 *
 * TODO: more flexible fps support (currently editors stick to const fps).
 */
class TimeItem : public QGraphicsRectItem {
public:
    TimeItem();
    virtual ~TimeItem() {
    }

    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
    QRectF boundingRect() const override;

    void move_to(core::Time time);
    void set_fps(core::Time::fps_type fps_);
    void set_readonly(bool ro);
    void set_pos_height(int y, int height);

protected:
    virtual void position_changed(core::Time time) = 0;
    virtual void selected_changed(bool /*is_selected*/) {}

private:
    double change_pos(double x);

private:
    core::Time::fps_type fps = 1;
};

} // namespace rainynite::studio

#endif
