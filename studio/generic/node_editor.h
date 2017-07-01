/*
 *  node_editor.h - abstract node editor widget class
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

#ifndef __STUDIO__NODE_EDITOR_H__8BFEEC92
#define __STUDIO__NODE_EDITOR_H__8BFEEC92

#include <memory>

#include <QWidget>

#include <core/actions/change_value.h>
#include <core/node/value.h>

#include "context_listener.h"

namespace core {
class AbstractValue;
}

namespace studio {

class NodeEditor {
public:
    virtual ~NodeEditor() {
        node_connection.disconnect();
    }
public:
    virtual void node_update() {
    }
public:
    void set_node(std::shared_ptr<core::AbstractValue> node_);
    inline std::shared_ptr<core::AbstractValue> get_node() {
        return node;
    }
private:
    std::shared_ptr<core::AbstractValue> node = nullptr;
    boost::signals2::connection node_connection;
};

template <class W, typename T>
class NodeEditorWidget : public NodeEditor, public ContextListener, public W {
public:
    using Widget = W;
    using ValueType = T;
public:
    NodeEditorWidget(QWidget* parent = nullptr) :
        NodeEditor(),
        ContextListener(),
        W(parent)
    {
        QObject::connect(
            this,
            &W::editingFinished,
            [this]() {
                if (auto vnode = dynamic_cast<core::Value<ValueType>*>(value_node)) {
                    if (vnode->mod() == W::value())
                        return;
                    get_context()->action_stack()->template emplace<core::actions::ChangeValue>(get_node(), W::value());
                }
            }
        );
    }
public:
    void node_update() override {
        if (value_node = dynamic_cast<core::BaseValue<ValueType>*>(get_node().get())) {
            this->update_value(value_node->get(get_time()));
            this->setReadOnly(!dynamic_cast<core::Value<ValueType>*>(value_node));
        }
    }
    void time_changed(core::Time time_) override {
        ContextListener::time_changed(time_);
        if (value_node = dynamic_cast<core::BaseValue<ValueType>*>(this->get_node().get()))
            this->update_value(value_node->get(get_time()));
    }
protected:
    core::BaseValue<ValueType>* value_node = nullptr;
};

} // namespace studio

#endif
