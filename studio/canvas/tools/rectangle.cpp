/*  rectangle.cpp - draw rectangle tool
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

#include <QRubberBand>

#include <core/node/abstract_value.h>
#include <core/node/node.h>
#include <core/node/make.h>
#include <core/node_info/node_info.h>

#include <geom_helpers/rectangle.h>

#include <util/geom.h>
#include "shape.h"

namespace rainynite::studio::tools {

/**
 * Rectangle draw tool.
 *
 * TODO: merge common code with zoom_area tool
 */
class Rectangle : public Shape {
public:
    string icon() const override {
        return "draw-rectangle";
    }

    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Rectangle";
    }

protected:
    bool mouse_press(QPoint const& pos) override {
        new_shape_at(
            get_canvas()->mapToScene(pos),
            [] (Geom::Point start_pos) {
                using namespace core;
                using util::point;
                using Geom::Rectangle, Geom::Point;
                auto rect_node = make_node_with_name_as<Node<Rectangle>>("RectangleAB");
                rect_node->set_property("a", make_value<Point>(start_pos));
                rect_node->set_property("b", make_value<Point>(start_pos));
                return rect_node;
            }
        );
        if (get_index())
            get_canvas()->set_active_node(get_index());
        return false;
    }
};

} // namespace rainynite::studio::tools

namespace rainynite::studio {
REGISTER_CANVAS_TOOL(tools::Rectangle, Canvas, 0x12);
} // namespace rainynite::studio
