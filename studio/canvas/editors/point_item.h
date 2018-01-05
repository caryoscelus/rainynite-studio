/*  point_item.h - point item reporting its position changes
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

#ifndef STUDIO_CANVAS_EDITORS_POINT_ITEM_H_8B768D2E_AB91_5E39_9D6E_8B2F45174CB7
#define STUDIO_CANVAS_EDITORS_POINT_ITEM_H_8B768D2E_AB91_5E39_9D6E_8B2F45174CB7

#include <QGraphicsEllipseItem>

namespace rainynite::studio {

class PointItemListener {
public:
    virtual void point_moved(size_t point_id, QPointF const& pos) = 0;
    virtual void point_stopped_moving(size_t point_id) = 0;
};

/**
 * On-canvas item for displaying "point" handles.
 *
 * Basically, it's a circle with the following properties:
 * - has its position in center, not lefttop corner
 * - automatically scales to stay the same size (NOTE: works for one view only!)
 * - supports dragging that can be turned on and off
 * - reports its new position when drag is over
 */
class AbstractPointItem : public QGraphicsEllipseItem {
public:
    AbstractPointItem();

    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;

    void set_readonly(bool ro);
    void set_pos(double x, double y);

    void set_color(QColor const& color);

protected:
    QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    virtual void moved(QPointF const& pos) = 0;
    virtual void stopped_moving() = 0;

private:
    static const int radius = 3;
    bool recursion_protection = false;
};

/**
 * Point item that takes `parent` PointItemListener and calls methods on it.
 */
class ListenerPointItem : public AbstractPointItem {
public:
    ListenerPointItem(PointItemListener* parent, size_t id_=0) :
        p(parent),
        id(id_)
    {}

protected:
    void moved(QPointF const& pos) override {
        p->point_moved(id, pos);
    }
    void stopped_moving() override {
        p->point_stopped_moving(id);
    }

private:
    observer_ptr<PointItemListener> p;
    size_t id;
};

} // namespace rainynite::studio

#endif
