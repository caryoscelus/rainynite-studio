/*  transform_move.cpp - a tool for creating move transform
 *  Copyright (C) 2018 caryoscelus
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
#include <core/actions/change_link.h>
#include <core/actions/change_value.h>
#include <core/action_stack.h>
#include <core/renderable.h>
#include <core/node_tree/transform.h>

#include <generic/process_node.h>
#include <util/geom.h>
#include "base.h"

namespace rainynite::studio {

struct AddTranslateTag {};

class AddTranslateProcessor : public ProcessNode<Geom::Point, AddTranslateTag> {
    using NodeRef = shared_ptr<core::BaseValue<Geom::Point>>;
public:
    NodeRef offset_node() const {
        return no_null(stored_node);
    }

protected:
    void set_offset_node(NodeRef offset_node_) {
        stored_node = offset_node_;
    }

private:
    NodeRef stored_node;
};

class AddTranslateToRenderable : public AddTranslateProcessor {
public:
    bool accept(core::NodeTreeIndex index) const override {
        auto node = get_context()->tree()->get_node(index);
        return node && node->get_type() == typeid(core::Renderable);
    }
    void feed(Geom::Point const& offset) override {
        using namespace core;
        auto idx = get_node_index();
        auto tree = get_context()->tree();
        auto node = tree->get_node(idx);
        auto transform_node = make_node_with_name_as<AbstractNode>("Transform", node, get_core_context());
        auto translate_node = make_node_with_name_as<AbstractNode>("Translate");
        set_offset_node(translate_node->get_property_as<Geom::Point>("offset"));
        offset_node()->set(offset);
        transform_node->set_property("transform", abstract_value_cast(std::move(translate_node)));
        auto action_stack = get_context()->action_stack();
        action_stack->emplace<core::actions::ChangeLink>(tree, idx, abstract_value_cast(std::move(transform_node)));
    }
};

REGISTER_PROCESS_NODE(AddTranslateToRenderable, AddTranslateProcessor)


namespace tools {

class TransformMove :
    public Base,
    FindTargetNode<AddTranslateProcessor>
{
public:
    string icon() const override {
        return "transform-move";
    }

    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Move transform";
    }

    bool canvas_event(QEvent* event) override {
        if (get_canvas() == nullptr)
            return false;
        if (auto ctx = get_canvas()->get_context()) {
            if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
                if (is_translating) {
                    auto delta = mouse_event->pos() - move_pos;
                    set_move_transform(delta);
                }
                switch (mouse_event->type()) {
                    case QEvent::MouseButtonPress: {
                        if (mouse_event->button() == Qt::LeftButton) {
                            move_pos = mouse_event->pos();
                            is_translating = add_move_transform();
                            return true;
                        }
                    } break;
                    case QEvent::MouseButtonRelease: {
                        if (mouse_event->button() == Qt::LeftButton) {
                            if (is_translating) {
                                close_action();
                                is_translating = false;
                            }
                            return true;
                        }
                    } break;
                    default:
                        break;
                }
            }
        }
        return Base::canvas_event(event);
    }

private:
    bool add_move_transform() {
        auto ctx = get_canvas()->get_context();
        if (target = find_appropriate_target(*no_null(ctx->tree()), ctx)) {
            target->feed({});
            return true;
        }
        return false;
    }

    void set_move_transform(QPointF const& delta) {
        auto action_stack = no_null(get_canvas()->get_context()->action_stack());
        if (target) {
            using core::actions::ChangeValue;
            action_stack->emplace<ChangeValue>(target->offset_node(), util::point(delta));
        }
    }

    void close_action() {
        auto action_stack = no_null(get_canvas()->get_context()->action_stack());
        action_stack->close();
    }

private:
    QPointF move_pos;
    bool is_translating = false;
    unique_ptr<AddTranslateProcessor> target;
};

} // namespace tools

REGISTER_CANVAS_TOOL(tools::TransformMove, Canvas, 0x20);

} // namespace rainynite::studio
