/*  point_item.h - point item reporting its position changes
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

#ifndef STUDIO_CANVAS_EDITORS_POINT_ITEM_H_8B768D2E_AB91_5E39_9D6E_8B2F45174CB7
#define STUDIO_CANVAS_EDITORS_POINT_ITEM_H_8B768D2E_AB91_5E39_9D6E_8B2F45174CB7

#include <QGraphicsEllipseItem>

namespace rainynite::studio {

/**
 * On-canvas item for displaying "point" handles.
 *
 * Basically, it's a circle with the following properties:
 * - has its position in center, not lefttop corner
 * - automatically scales to stay the same size (NOTE: works for one view only!)
 * - supports dragging that can be turned on and off
 */
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
public:
    void set_readonly(bool ro);
    void set_pos(double x, double y);
private:
    Callback position_callback;
};

} // namespace rainynite::studio

#endif
