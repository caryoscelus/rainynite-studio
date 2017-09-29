/*
 *  editor_context.h - editor Context
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

#ifndef __STUDIO__GENERIC__EDITOR_CONTEXT_H__4E809780
#define __STUDIO__GENERIC__EDITOR_CONTEXT_H__4E809780

#include <core/context.h>

namespace rainynite::core {
class AbstractValue;
class ActionStack;
}

namespace rainynite::studio {

class EditorContext {
public:
    EditorContext(shared_ptr<core::Context> context_) :
        context(context_)
    {}
public:
    inline shared_ptr<core::Context> get_context() const {
        return context;
    }
    void set_active_node(shared_ptr<core::AbstractValue> node);
    inline shared_ptr<core::AbstractValue> get_active_node() const {
        return active_node;
    }
    shared_ptr<core::ActionStack> action_stack();
public:
    inline boost::signals2::signal<void(core::Time)>& changed_time() {
        return context->changed_time;
    }
    inline boost::signals2::signal<void(core::Time::fps_type)>& changed_fps() {
        return context->changed_fps;
    }
    inline boost::signals2::signal<void(shared_ptr<core::AbstractValue>)>& changed_active_node() {
        return changed_active_node_;
    }
private:
    shared_ptr<core::Context> context;
    shared_ptr<core::AbstractValue> active_node;
private:
    boost::signals2::signal<void(shared_ptr<core::AbstractValue>)> changed_active_node_;
};

shared_ptr<EditorContext> global_dummy_context();

} // namespace rainynite::studio

#endif
