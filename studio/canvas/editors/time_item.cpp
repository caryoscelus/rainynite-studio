/*  time_item.cpp - time editing item
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

#include <QGuiApplication>
#include <QPalette>
#include <QPen>

#include "time_item.h"

namespace rainynite::studio {

TimeItem::TimeItem(Callback callback_) :
    QGraphicsRectItem {0, 0, 0.25, 80},
    callback(callback_)
{
    auto brush = QGuiApplication::palette().text();
    auto color = brush.color();
    color.setAlphaF(0.5);
    brush.setColor(color);
    setBrush(brush);
    setPen({Qt::NoPen});
}

QVariant TimeItem::itemChange(GraphicsItemChange change, QVariant const& value) {
    if (change == ItemPositionChange) {
        auto pos = value.toPointF();
        auto new_x = change_pos(pos.x());
        return QGraphicsItem::itemChange(change, QPointF{ new_x, y() });
    }
    return QGraphicsItem::itemChange(change, value);
}

void TimeItem::move_to(core::Time time) {
    auto s = time.get_seconds();
    setPos(s, 0);
}

void TimeItem::set_readonly(bool ro) {
    setFlag(QGraphicsItem::ItemIsMovable, !ro);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
}

void TimeItem::set_pos_height(int y, int height) {
    auto r = rect();
    r.setY(y);
    r.setHeight(height);
    setRect(r);
}

double TimeItem::change_pos(double x) {
    int frames = x * fps;
    callback(core::Time(0, fps, frames));
    return (double)frames / fps;
}

} // namespace rainynite::studio
