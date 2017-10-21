/*  shape.cpp - base for shape creating tools
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

#include <core/node_info.h>
#include <core/actions/list.h>
#include <core/actions/change_link.h>
#include <core/action_stack.h>
#include <core/exceptions.h>
#include <core/renderable.h>

#include "shape.h"

namespace rainynite::studio::tools {

bool Shape::canvas_event(QEvent* event) {
    if (get_canvas() == nullptr)
        return false;
    if (auto ctx = get_canvas()->get_context()) {
        if (auto node = ctx->get_active_node()) {
            if (accept(node)) {
                if (draw_shape_event(event))
                    return true;
            }
        }
    }
    return Base::canvas_event(event);
}

bool Shape::draw_shape_event(QEvent* event) {
    if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
        switch (mouse_event->type()) {
            case QEvent::MouseButtonPress: {
                if (mouse_event->button() == Qt::LeftButton) {
                    return mouse_press(mouse_event->pos());
                }
            } break;
            case QEvent::MouseMove: {
                return mouse_move(mouse_event->pos());
            } break;
            case QEvent::MouseButtonRelease: {
                if (mouse_event->button() == Qt::LeftButton) {
                    return mouse_release(mouse_event->pos());
                }
            } break;
            default:
                break;
        }
    }
    return false;
}

bool Shape::accept(shared_ptr<core::AbstractValue> node) {
    // TODO: modularize, support replacing shapes
    using namespace core;
    auto name = node_name(*node);
    return name == "RenderShape"
        || name == "Composite"
        || node->get_type() == typeid(vector<Renderable>);
}

void Shape::write_shape(shared_ptr<core::AbstractValue> shape) {
    using namespace core;

    auto action_stack = get_canvas()->get_context()->action_stack();
    if (action_stack == nullptr)
        throw NullPointerException("Cannot write to empty action stack.");

    auto value = get_canvas()->get_context()->get_active_node();
    auto name = node_name(*value);
    auto node = dynamic_pointer_cast<AbstractNode>(value);

    // TODO: modularize, support replacing shapes
    if (name == "RenderShape") {
        action_stack->emplace<actions::SetProperty>(node, "shape", shape);
    } else if (value->get_type() == typeid(vector<Renderable>)) {
        if (auto layers = dynamic_pointer_cast<AbstractListLinked>(std::move(value))) {
            add_renderable_to_list(action_stack, layers, shape);
        } else {
            throw std::runtime_error("Has vector<Renderable> type, but isn't list: "+name);
        }
    } else if (name == "Composite") {
        auto layers_node = node->get_property("layers");
        if (auto layers = dynamic_pointer_cast<AbstractListLinked>(layers_node)) {
            return add_renderable_to_list(action_stack, layers, shape);
        } else {
            throw std::runtime_error("Can't add layer to Composite node");
        }
    } else {
        throw std::runtime_error("Can't add shape to "+name);
    }
}

void Shape::add_renderable_to_list(shared_ptr<core::ActionStack> action_stack, shared_ptr<core::AbstractListLinked> layers, shared_ptr<core::AbstractValue> shape) {
    using namespace core;
    auto render_shape = make_node_with_name<RenderableNode>("RenderShape");
    render_shape->set_property("shape", shape);
    action_stack->emplace<actions::ListPush>(layers, render_shape);
}

} // namespace rainynite::studio::tools
