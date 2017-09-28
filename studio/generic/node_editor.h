/*  node_editor.h - abstract node editor widget class
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

#ifndef STUDIO_GENERIC_NODE_EDITOR_H_81406495_5D02_5256_A039_914B9CE337E8
#define STUDIO_GENERIC_NODE_EDITOR_H_81406495_5D02_5256_A039_914B9CE337E8

#include <core/std/memory.h>

#include <QWidget>

#include <core/actions/change_value.h>
#include <core/node/value.h>
#include <core/serialize/exceptions.h>

#include "context_listener.h"

namespace rainynite::core {
class AbstractValue;
}

namespace rainynite::studio {

class NodeEditor : public ContextListener {
public:
    virtual ~NodeEditor() {
        node_connection.disconnect();
    }
public:
    virtual void node_update() {
    }
public:
    void set_node(shared_ptr<core::AbstractValue> node_);
    inline shared_ptr<core::AbstractValue> get_node() {
        return node;
    }
public:
    template <typename T>
    optional<T> get_value() {
        if (auto node = get_node_as<T>()) {
            return node->value(get_core_context());
        }
        return {};
    }
    template <typename T>
    shared_ptr<core::BaseValue<T>> get_node_as() {
        return dynamic_pointer_cast<core::BaseValue<T>>(std::move(get_node()));
    }
private:
    shared_ptr<core::AbstractValue> node = nullptr;
    boost::signals2::connection node_connection;
};

class NodeEditorShowChildren {
public:
    virtual bool operator()() const = 0;
};

#define REGISTER_NODE_EDITOR_SHOW_CHILDREN(Name, node_name, value) \
class Name##ShowChildren : \
    public NodeEditorShowChildren, \
    private class_init::StringRegistered<Name##ShowChildren, NodeEditorShowChildren> \
{ \
public: \
    static std::string name() { \
        return node_name; \
    } \
public: \
    bool operator()() const override { \
        return value; \
    } \
}

} // namespace rainynite::studio

#endif
