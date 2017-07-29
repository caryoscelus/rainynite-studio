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

#include <QGuiApplication>

#include <2geom/point.h>

#include <widgets/canvas.h>
#include <util/pen.h>
#include "point_item.h"

namespace studio {

PointItem::PointItem(Callback callback) :
    QGraphicsEllipseItem(-radius, -radius, radius*2, radius*2),
    position_callback(callback)
{
    setPen(pens::cosmetic_solid());
    setBrush(QGuiApplication::palette().text());
}

void PointItem::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
    // Set correct size if view was zooomed..
    // NOTE: this dirty hack relies on the scene being used only by one view
    auto transform = painter->transform();
    auto r = radius/transform.m11();
    setRect(-r, -r, r*2, r*2);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

QVariant PointItem::itemChange(GraphicsItemChange change, QVariant const& value) {
    if (change == ItemPositionHasChanged) {
        position_callback(pos().x(), pos().y());
    }
    return QGraphicsItem::itemChange(change, value);
}

void PointItem::set_readonly(bool ro) {
    setFlag(QGraphicsItem::ItemIsMovable, !ro);
    setFlag(QGraphicsItem::ItemIsSelectable, !ro);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
}

void PointItem::set_pos(double x, double y) {
    setPos(x, y);
}

} // namespace studio