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
#include <core/node/node.h>
#include <core/node/make.h>
#include <core/node_info.h>
#include <core/action_stack.h>
#include <core/actions/change_value.h>
#include <core/exceptions.h>

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
            if (inplace) {
                old_path = path_node->mod();
                path_node->set({});
            } else {
                new_shape_at(
                    get_canvas()->mapToScene(pos),
                    [this] (Geom::Point start_pos) {
                        path_node = make_shared<core::Value<Geom::BezierKnots>>();
//                         path_node
                        return path_node;
                    }
                );
            }
            path.reset(&path_node->mod());
            path->closed = false;
            auto editors = add_canvas_node_editor(*get_canvas(), get_index());
//             editor = make_unique<BezierEditor>();
//             editor->set_canvas(get_canvas());
//             editor->set_node(get_index());
            dynamic_pointer_cast<BezierEditor>(editors.front())->set_appending(true);
//             return false;
//             path->emplace_back(convert_pos(pos));
        }
        is_pressed = true;
        return false;
    }
    bool editing_done() override {
        is_drawing = false;
        return false;
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

    bool accept(shared_ptr<core::AbstractValue> node) override {
        // TODO: universal fix
        if (Shape::accept(node))
            return true;
        if (auto bezier_node = dynamic_pointer_cast<core::Value<Geom::BezierKnots>>(std::move(node))) {
            path_node = std::move(bezier_node);
            inplace = true;
            return true;
        }
        return false;
    }

    void write_shape(shared_ptr<core::AbstractValue> shape) override {
        if (inplace) {
            auto action_stack = get_canvas()->get_context()->action_stack();
            if (action_stack == nullptr)
                throw NullPointerException("Cannot write to empty action stack.");
            // restore old value for undo
            auto new_path = path_node->mod();
            path_node->set(old_path);
            action_stack->emplace<core::actions::ChangeValue>(path_node, new_path);
            action_stack->close();
        } else {
            Shape::write_shape(shape);
        }
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
    Geom::BezierKnots old_path;
    bool inplace = false;
    observer_ptr<Geom::BezierKnots> path;
    Geom::Point next_pos;
};

} // namespace rainynite::studio::tools


namespace rainynite::studio {

REGISTER_CANVAS_TOOL(tools::Bezier, Canvas, 0x10);

} // namespace rainynite::studio
