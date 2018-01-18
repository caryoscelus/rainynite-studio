/*  node_editor.h - abstract node editor class
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

#ifndef STUDIO_GENERIC_NODE_EDITOR_H_81406495_5D02_5256_A039_914B9CE337E8
#define STUDIO_GENERIC_NODE_EDITOR_H_81406495_5D02_5256_A039_914B9CE337E8

#include <QWidget>

#include <core/std/memory.h>
#include <core/all_types.h>
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

    void set_update_enabled(bool value) {
        update_enabled = value;
    }

    virtual void node_update() {
    }

    void set_node(core::NodeTree::Index index);

    /// Make node associated with this editor active/current
    void activate_node() {
        get_context()->set_active_node(get_node_index());
    }

    shared_ptr<core::AbstractValue> get_node() const {
        return get_context()->get_node(get_node_index());
    }

    core::NodeTree::Index get_node_index() const {
        return node_index;
    }

    template <typename T>
    optional<T> get_value() const {
        if (auto node = get_node_as<T>()) {
            return node->value(get_core_context());
        }
        return {};
    }

    template <typename T>
    shared_ptr<core::BaseValue<T>> get_node_as() const {
        return dynamic_pointer_cast<core::BaseValue<T>>(get_node());
    }

    void write_value(any value);
    void close_action();

protected:
    bool update_enabled = true;

private:
    core::NodeTree::Index node_index;
    boost::signals2::connection node_connection;
};

/// Calculate transform helper
Geom::Affine get_transform(NodeEditor const& editor);

class NodeEditorShowChildren {
public:
    virtual bool operator()() const = 0;
};

#define REGISTER_NODE_EDITOR_SHOW_CHILDREN_NAMED(Name, node_name, value) \
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

#define REGISTER_NODE_EDITOR_SHOW_CHILDREN(Name, value) \
REGISTER_NODE_EDITOR_SHOW_CHILDREN_NAMED(Name, #Name, value)

#define REGISTER_TEMPLATE_NODE_EDITOR_SHOW_CHILDREN(Name, node_name, value) \
template <typename T> \
class Name##ShowChildren : \
    public NodeEditorShowChildren, \
    private class_init::StringRegistered< \
        Name##ShowChildren<T>, \
        NodeEditorShowChildren \
    > \
{ \
public: \
    static std::string name() { \
        return node_name+string("/")+core::get_primitive_type(typeid(T))(); \
    } \
public: \
    bool operator()() const override { \
        return value; \
    } \
}; \
TYPE_INSTANCES(Name##ShowChildren)

} // namespace rainynite::studio

#endif
