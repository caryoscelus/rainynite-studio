/*  bezier.cpp - draw bezier tool
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

#include <QGraphicsPathItem>

#include <core/node/abstract_value.h>
#include <core/node/node.h>
#include <core/node/make.h>
#include <core/node_info.h>

#include <geom_helpers/knots.h>

#include <canvas/editors/bezier_editor.h>
#include <util/pen.h>
#include <util/geom.h>
#include "shape.h"

namespace rainynite::studio::tools {

class Bezier : public Shape {
public:
    string icon() const override {
        return "draw-path";
    }

    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Bezier";
    }

protected:
    void setup_canvas() override {
        get_canvas()->viewport()->setMouseTracking(true);
    }

    bool mouse_press(QPoint const& pos) override {
        if (!is_drawing) {
            is_drawing = true;
            path_node = make_shared<core::Value<Geom::BezierKnots>>();
            path.reset(&path_node->mod());
            path->closed = false;
            editor = make_unique<BezierEditor>();
            editor->set_canvas(get_canvas());
            editor->set_node(path_node);
            path->emplace_back(convert_pos(pos));
        }
        is_pressed = true;
        return true;
    }
    bool mouse_move(QPoint const& pos) override {
        if (is_drawing) {
            auto p = convert_pos(pos);
            auto& knot = path->last();
            if (is_pressed) {
                knot.tg1 = knot.pos-p;
                knot.tg2 = p-knot.pos;
                editor->redraw();
            } else {
                knot.pos = p;
                editor->redraw();
            }
        }
        return is_drawing;
    }
    bool mouse_release(QPoint const& pos) override {
        if (is_drawing) {
            path->emplace_back(convert_pos(pos));
            is_pressed = false;
            return true;
        }
        return false;
    }
    bool editing_done() override {
        if (is_drawing) {
            // Remove last knot that appears due to double-click..
            path->knots.pop_back();
            // ..and the one created by mouse move
            path->knots.pop_back();
            write_shape(path_node);
            editor.reset();
            is_drawing = false;
            return true;
        }
        return false;
    }

private:
    Geom::Point convert_pos(QPoint const& pos) {
        return util::point(get_canvas()->mapToScene(pos));
    }

private:
    bool is_drawing = false;
    bool is_pressed = false;
    unique_ptr<BezierEditor> editor;
    shared_ptr<core::Value<Geom::BezierKnots>> path_node;
    observer_ptr<Geom::BezierKnots> path;
    Geom::Point next_pos;
};

} // namespace rainynite::studio::tools


namespace rainynite::studio {

REGISTER_CANVAS_TOOL(tools::Bezier, Canvas);

} // namespace rainynite::studio
