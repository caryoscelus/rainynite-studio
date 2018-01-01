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
#include <core/node_info.h>

#include <geom_helpers/rectangle.h>

#include "shape.h"

namespace rainynite::studio {
namespace tools {

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
        rect_start = pos;
        rubber_band = new QRubberBand(QRubberBand::Rectangle, get_canvas());
        rubber_band->setGeometry({rect_start, rect_start});
        rubber_band->show();
        return is_drawing = true;
    }
    bool mouse_move(QPoint const& pos) override {
        if (is_drawing) {
            rubber_band->setGeometry(get_rect(rect_start, pos));
        }
        return is_drawing;
    }
    bool mouse_release(QPoint const& pos) override {
        if (is_drawing) {
            auto rect = get_rect(rect_start, pos);
            rubber_band->setGeometry(rect);
            rectangle_drawn(
                get_canvas()->mapToScene(rect_start),
                get_canvas()->mapToScene(pos)
            );
            delete rubber_band;
            is_drawing = false;
            return true;
        }
        return false;
    }

    void rectangle_drawn(QPointF a, QPointF b) {
        auto rect_node = core::make_node_with_name<core::Node<Geom::Rectangle>>("RectangleAB");
        rect_node->set_property("a", core::make_value<Geom::Point>(a.x(), a.y()));
        rect_node->set_property("b", core::make_value<Geom::Point>(b.x(), b.y()));
        write_shape(rect_node);
    }

private:
    QRect get_rect(QPoint a, QPoint b) {
        return {
            std::min(a.x(), b.x()),
            std::min(a.y(), b.y()),
            std::abs(a.x()-b.x()),
            std::abs(a.y()-b.y())
        };
    }

private:
    bool is_drawing = false;
    QPoint rect_start;
    QRubberBand* rubber_band;
};

} // namespace tools

REGISTER_CANVAS_TOOL(tools::Rectangle, Canvas, 0x12);

} // namespace rainynite::studio
