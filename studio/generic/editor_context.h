/*  editor_context.h - editor Context
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

#ifndef STUDIO_GENERIC_EDITOR_CONTEXT_H_E5FE1C55_3375_5F79_8BBB_5D6A55CA2AA0
#define STUDIO_GENERIC_EDITOR_CONTEXT_H_E5FE1C55_3375_5F79_8BBB_5D6A55CA2AA0

#include <core/std/string.h>
#include <core/context.h>
#include <core/node_tree/node_tree.h>

namespace rainynite::core {
class AbstractValue;
class ActionStack;
}

namespace rainynite::studio {

class EditorContext {
public:
    template <typename... Args>
    using Signal = boost::signals2::signal<Args...>;

    EditorContext(shared_ptr<core::Context> context_) :
        context(context_)
    {}

    shared_ptr<core::Context> get_context() const {
        return context;
    }

    void set_active_node(core::NodeTree::Index index);

    core::NodeTree::Index get_active_node_index() const {
        return active_node;
    }

    shared_ptr<core::AbstractValue> get_active_node() const {
        return get_node(get_active_node_index());
    }

    template <class T>
    shared_ptr<T> get_active_node_as() const {
        return dynamic_pointer_cast<T>(get_active_node());
    }

    shared_ptr<core::AbstractValue> get_node(core::NodeTree::Index index) const {
        if (!index)
            return nullptr;
        if (auto node_tree = tree())
            return node_tree->get_node(index);
        return nullptr;
    }

    // Get tree
    shared_ptr<core::NodeTree> tree() const;

    /// Get action stack
    shared_ptr<core::ActionStack> action_stack() const;

    void set_file_name(string const& fname) {
        file_name = fname;
    }

    string get_file_name() const {
        return file_name;
    }

    Signal<void(core::Time)>& changed_time() {
        return context->changed_time;
    }

    Signal<void(core::Time::fps_type)>& changed_fps() {
        return context->changed_fps;
    }

    Signal<void(core::NodeTree::Index)>& changed_active_node() {
        return changed_active_node_;
    }

    Signal<void(bool)> playback_change;

private:
    shared_ptr<core::Context> context;
    core::NodeTree::Index active_node;
    string file_name;

    Signal<void(core::NodeTree::Index)> changed_active_node_;
};

shared_ptr<EditorContext> global_dummy_context();

} // namespace rainynite::studio

#endif
