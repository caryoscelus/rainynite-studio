/*  circle.cpp - draw circle tool
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

#include <QGraphicsEllipseItem>

#include <core/node/abstract_value.h>
#include <core/node/node.h>
#include <core/node/make.h>
#include <core/node_info.h>

#include <geom_helpers/circle.h>

#include <util/pen.h>
#include "shape.h"

namespace rainynite::studio {
namespace tools {

class Circle : public Shape {
public:
    string icon() const override {
        return "draw-circle";
    }

    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Circle";
    }

protected:
    bool mouse_press(QPoint const& pos) override {
        circle_center = pos;
        preview.reset(get_scene()->addEllipse({}, pens::cosmetic_dash()));
        return is_drawing = true;
    }
    bool mouse_move(QPoint const& pos) override {
        if (is_drawing) {
            preview->setRect(get_rect(
                get_canvas()->mapToScene(circle_center),
                get_canvas()->mapToScene(pos)
            ));
        }
        return is_drawing;
    }
    bool mouse_release(QPoint const& pos) override {
        if (is_drawing) {
            circle_drawn(
                get_canvas()->mapToScene(circle_center),
                get_canvas()->mapToScene(pos)
            );
            get_scene()->removeItem(preview.get());
            preview.reset();
            is_drawing = false;
            return true;
        }
        return false;
    }

private:
    void circle_drawn(QPointF a, QPointF b) {
        using namespace core;
        auto circle_node = make_node_with_name<Node<Geom::Circle>>("CirclePR");
        circle_node->set_property("position", make_value<Geom::Point>(a.x(), a.y()));
        auto radius = get_radius(a, b);
        circle_node->set_property("radius", make_value<double>(radius));
        write_shape(circle_node);
    }

    double get_radius(QPointF const& a, QPointF const& b) {
        auto vec = a-b;
        return Geom::Point(vec.x(), vec.y()).length();
    }

    QRectF get_rect(QPointF const& center, QPointF const& end) {
        auto radius = get_radius(center, end);
        return {
            center-QPointF(radius, radius),
            center+QPointF(radius, radius)
        };
    }

private:
    bool is_drawing = false;
    QPoint circle_center;
    observer_ptr<QGraphicsEllipseItem> preview;
};

} // namespace tools

REGISTER_CANVAS_TOOL(tools::Circle, Canvas);

} // namespace rainynite::studio
