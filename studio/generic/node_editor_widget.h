/*  node_editor_widget.h - convert widget to node editor
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

#ifndef STUDIO_GENERIC_NODE_EDITOR_WIDGET_H_825E9422_9018_587A_82D1_1ECB634E35EF
#define STUDIO_GENERIC_NODE_EDITOR_WIDGET_H_825E9422_9018_587A_82D1_1ECB634E35EF

#include <experimental/type_traits>

#include <core/type_info.h>

#include "node_editor.h"

namespace rainynite::studio {

namespace detail {

template <class C>
using editing_finished_t = decltype(std::declval<C&>().editingFinished());

template <class C>
constexpr bool has_editing_finished_signal = std::experimental::is_detected_v<editing_finished_t, C>;

}

/**
 * Abstract template for "converting" editor widget classes into node editors.
 *
 * It is currently not used directly, but provides common base for
 * UntypedNodeEditorWidget and NodeEditorWidget.
 */
template <class S, typename T>
class AbstractNodeEditorWidget : public NodeEditor {
public:
    /// Curiously-recurring pattern class argument
    using Self = S;
    /// Type to store value node in (not pointer!)
    using ValueNodeType = T;

    /**
     * This function should be called from subclass ctr
     *
     * It cannot be called from ctr here because this is not yet Self*
     */
    void init() {
        if constexpr (detail::has_editing_finished_signal<Self>) {
            QObject::connect(
                self(),
                &Self::editingFinished,
                self(),
                &Self::write_action
            );
        }
    }

    void node_update() override {
        if (value_node = dynamic_cast<ValueNodeType*>(this->get_node().get())) {
            self()->do_update_value(get_core_context());
            self()->setReadOnly(self()->is_readonly(value_node));
        }
    }
    void time_changed(core::Time time_) override {
        ContextListener::time_changed(time_);
        if (value_node = dynamic_cast<ValueNodeType*>(this->get_node().get()))
            self()->do_update_value(get_core_context());
    }

private:
    inline Self* self() {
        static_assert(std::is_base_of_v<AbstractNodeEditorWidget, Self>);
        return static_cast<Self*>(this);
    }

protected:
    ValueNodeType* value_node = nullptr;
};

/**
 * This template turns string-editing `Widget` into node-editing widget
 */
template <class W>
class UntypedNodeEditorWidget :
    public AbstractNodeEditorWidget<
        UntypedNodeEditorWidget<W>,
        core::AbstractValue
    >,
    public W
{
public:
    using Widget = W;
    using ValueNodeType = core::AbstractValue;

    UntypedNodeEditorWidget(QWidget* parent = nullptr) :
        Widget(parent)
    {
        this->init();
    }

    /// This function is called when we should write user changes back to node
    void write_action() {
        if (this->value_node == nullptr)
            return;
        auto s = Widget::value();
        any value;
        try {
            value = core::parse_primitive_type(this->value_node->get_type(), s);
        } catch (core::serialize::DeserializationError const&) {
            // TODO
            return;
        } catch (std::exception const&) {
            // TODO
            return;
        }
        this->get_context()->action_stack()->template emplace<core::actions::ChangeValue>(this->get_node(), value);
        this->get_context()->action_stack()->close();
    }

    /// This function forwards update_value call
    void do_update_value(shared_ptr<core::Context> ctx) {
        this->update_value(this->value_node->get_any(std::move(ctx)));
    }

    bool is_readonly(ValueNodeType* /*node*/) const {
        return false;
    }
};

/**
 * This template turns `Widget` into node-editing widget with specific type.
 */
template <class W, typename T>
class NodeEditorWidget :
    public AbstractNodeEditorWidget<
        NodeEditorWidget<W,T>,
        core::BaseValue<T>
    >,
    public W
{
public:
    using Widget = W;
    using ValueType = T;
    using ValueNodeType = core::BaseValue<T>;

    NodeEditorWidget(QWidget* parent = nullptr) :
        Widget(parent)
    {
        this->init();
    }

    void write_action() {
        if (this->value_node == nullptr)
            return;
        if (auto vnode = dynamic_cast<core::Value<ValueType>*>(this->value_node)) {
            if (vnode->mod() == Widget::value())
                return;
        }
        this->get_context()->action_stack()->template emplace<core::actions::ChangeValue>(this->get_node(), Widget::value());
        this->get_context()->action_stack()->close();
    }

    /// This function forwards update_value call
    void do_update_value(shared_ptr<core::Context> ctx) {
        this->update_value(this->value_node->value(std::move(ctx)));
    }

    bool is_readonly(ValueNodeType* node) const {
        return dynamic_cast<core::Value<ValueType>*>(node) == nullptr;
    }
};

} // namespace rainynite::studio

#endif
