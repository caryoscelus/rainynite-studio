/*
 *  point_item.cpp - point item reporting its position changes
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

#include <2geom/point.h>

#include <widgets/canvas.h>
#include "point_item.h"

namespace studio {

PointItem::PointItem(Callback callback) :
    QGraphicsEllipseItem(0, 0, radius*2, radius*2),
    position_callback(callback)
{}

void PointItem::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
    auto transform = painter->transform();
    auto offset = transform.map(QPointF());
    painter->resetTransform();
    painter->translate(offset.x()-radius, offset.y()-radius);
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->setTransform(transform);
}

QVariant PointItem::itemChange(GraphicsItemChange change, QVariant const& value) {
    if (change == ItemPositionHasChanged) {
        auto pos = value.toPointF();
        position_callback(pos.x(), pos.y());
    }
    return QGraphicsItem::itemChange(change, value);
}

void PointItem::set_readonly(bool ro) {
    setFlag(QGraphicsItem::ItemIsMovable, !ro);
    setFlag(QGraphicsItem::ItemIsSelectable, !ro);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
}

void PointItem::set_pos(double x, double y) {
    setPos({x, y});
}

} // namespace studio
