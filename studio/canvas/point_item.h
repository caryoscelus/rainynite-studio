/*
 *  point_item.h - point item reporting its position changes
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

#ifndef __STUDIO__CANVAS__POINT_ITEM_H__C2FC4E6E
#define __STUDIO__CANVAS__POINT_ITEM_H__C2FC4E6E

#include <QGraphicsEllipseItem>

namespace studio {

class PointItem : public QGraphicsEllipseItem {
public:
    using Callback = std::function<void(double, double)>;
public:
    static const int radius = 2;
public:
    PointItem(Callback callback);
public:
    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;
    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
    void set_readonly(bool ro);
    void set_pos(double x, double y);
private:
    Callback position_callback;
};

} // namespace studio

#endif
