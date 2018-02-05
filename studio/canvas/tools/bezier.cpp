/*  bezier.cpp - draw bezier tool
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

#include <QGraphicsPathItem>

#include <core/node/abstract_value.h>
#include <core/node/make.h>
#include <core/node_info/node_info.h>
#include <core/action_stack.h>
#include <core/actions/change_value.h>
#include <core/util/exceptions.h>

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
        is_drawing = false;
    }

    bool mouse_press(QPoint const& pos) override {
        if (!is_drawing) {
            is_drawing = true;
            new_shape_at(
                get_canvas()->mapToScene(pos),
                [this] (Geom::Point /*start_pos*/) {
                    path_node = make_shared<core::Value<Geom::BezierKnots>>();
                    return path_node;
                }
            );
            path.reset(&path_node->mod());
            path->closed = false;
            auto editors = add_canvas_node_editor(*get_canvas(), get_index());
            dynamic_pointer_cast<BezierEditor>(editors.front())->set_appending(true);
        }
        return false;
    }
    bool editing_done() override {
        is_drawing = false;
        return false;
    }

private:
    Geom::Point convert_pos(QPoint const& pos) {
        return util::point(get_canvas()->mapToScene(pos));
    }

private:
    bool is_drawing = false;
    shared_ptr<core::Value<Geom::BezierKnots>> path_node;
    observer_ptr<Geom::BezierKnots> path;
};

} // namespace rainynite::studio::tools


namespace rainynite::studio {

REGISTER_CANVAS_TOOL(tools::Bezier, Canvas, 0x10);

} // namespace rainynite::studio
