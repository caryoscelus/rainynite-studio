/*  shape.cpp - base for shape creating tools
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

#include <core/node_info/node_info.h>
#include <core/actions/list.h>
#include <core/actions/change_link.h>
#include <core/action_stack.h>
#include <core/util/exceptions.h>
#include <core/renderable.h>
#include <core/node_tree/transform.h>

#include <util/geom.h>
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
    return false;
    return Base::canvas_event(event);
}

void Shape::new_shape_at(QPointF const& pos, MakeShape f) {
    if (auto ctx = get_canvas()->get_context()) {
        if (auto node = ctx->get_active_node_index()) {
            if (auto node_tree = ctx->tree()) {
                auto affine = core::get_transform(ctx->get_context(), *node_tree, node);
                auto node = f(util::point(pos) * affine.inverse());
                write_shape(node);
            }
        }
    }
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
            case QEvent::MouseButtonDblClick: {
                if (mouse_event->button() == Qt::LeftButton) {
                    return editing_done();
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

    auto ctx = get_canvas()->get_context();
    if (ctx == nullptr)
        throw NullPointerException("Cannot write when context is empty.");

    auto tree = get_canvas()->get_context()->tree();
    if (tree == nullptr)
        throw NullPointerException("Tree is null");

    auto node_index = ctx->get_active_node_index();
    auto value = ctx->get_active_node();

    auto name = node_name(*value);
    auto node = dynamic_pointer_cast<AbstractNode>(value);

    auto add_renderable_to_list = [this, &tree, &action_stack](auto layers, auto layers_index, auto shape) {
        using namespace core;
        auto render_shape = make_node_with_name_as<AbstractNode>("RenderShape");
        render_shape->set_property("shape", shape);
        auto layers_path = tree_index_to_path(*tree, layers_index);
        action_stack->emplace<actions::ListPush>(tree, layers_index, abstract_value_cast(render_shape));
        target_node_index = tree_index_to_path(*tree, tree->index(tree_path_to_index(*tree, layers_path), layers->link_count()-1));
    };

    // TODO: modularize, support replacing shapes
    if (name == "RenderShape") {
        action_stack->emplace<actions::SetProperty>(tree, node_index, "shape", shape);
        target_node_index = tree_index_to_path(*tree, tree->index(node_index, node->get_name_id("shape")));
    } else if (value->get_type() == typeid(vector<Renderable>)) {
        if (auto layers = list_cast(std::move(value))) {
            add_renderable_to_list(layers, node_index, shape);
        } else {
            throw std::runtime_error("Has vector<Renderable> type, but isn't list: "+name);
        }
    } else if (name == "Composite") {
        auto layers_node = node->get_property("layers");
        if (auto layers = list_cast(layers_node)) {
            auto layers_index = tree->index(node_index, node->get_name_id("layers"));
            return add_renderable_to_list(layers, layers_index, shape);
        } else {
            throw std::runtime_error("Can't add layer to Composite node");
        }
    } else {
        throw std::runtime_error("Can't add shape to "+name);
    }
}

core::NodeTreeIndex Shape::get_index() const {
    auto tree = no_null(get_canvas()->get_context()->tree());
    return tree_path_to_index(*tree, target_node_index);
}

} // namespace rainynite::studio::tools
