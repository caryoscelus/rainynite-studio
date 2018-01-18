/*  point_item.cpp - point item reporting its position changes
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

#include <QGuiApplication>

#include <2geom/point.h>

#include <widgets/canvas.h>
#include <util/pen.h>
#include "point_item.h"

namespace rainynite::studio {

AbstractPointItem::AbstractPointItem() :
    QGraphicsEllipseItem(-radius, -radius, radius*2, radius*2)
{
    setPen(pens::cosmetic_solid());
    setBrush(QGuiApplication::palette().text());
}

void AbstractPointItem::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
    // Set correct size if view was zooomed..
    // NOTE: this dirty hack relies on the scene being used only by one view
    auto t = painter->transform();
    auto rx = radius / std::sqrt(t.m11()*t.m11() + t.m21()*t.m21());
    auto ry = radius / std::sqrt(t.m22()*t.m22() + t.m12()*t.m12());
    setRect(-rx, -ry, rx*2, ry*2);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

QVariant AbstractPointItem::itemChange(GraphicsItemChange change, QVariant const& value) {
    if (change == ItemPositionChange && !recursion_protection) {
        recursion_protection = true;
        auto new_pos = value.toPointF();
        if (new_pos != pos())
            moved(new_pos);
    } else if (change == ItemPositionHasChanged) {
        recursion_protection = false;
    }
    return QGraphicsItem::itemChange(change, value);
}

void AbstractPointItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsEllipseItem::mouseReleaseEvent(event);
    stopped_moving();
}

void AbstractPointItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
    double_clicked();
}

void AbstractPointItem::set_readonly(bool ro) {
    setFlag(QGraphicsItem::ItemIsMovable, !ro);
    setFlag(QGraphicsItem::ItemIsSelectable, !ro);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
}

void AbstractPointItem::set_pos(double x, double y) {
    setPos(x, y);
}

void AbstractPointItem::set_color(QColor const& color) {
    setBrush(color);
}

} // namespace rainynite::studio
