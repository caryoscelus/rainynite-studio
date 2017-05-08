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
#include <core/action.h>

namespace studio {

class EditorContext {
public:
    EditorContext(std::shared_ptr<core::Context> context_) :
        context(context_)
    {}
public:
    inline std::shared_ptr<core::Context> get_context() const {
        return context;
    }
    void set_active_node(std::shared_ptr<core::AbstractValue> node);
    inline core::ActionStack& action_stack() {
        return action_stack_;
    }
public:
    inline boost::signals2::signal<void(core::Time)>& changed_time() {
        return context->changed_time;
    }
    inline boost::signals2::signal<void(std::shared_ptr<core::AbstractValue>)>& changed_active_node() {
        return changed_active_node_;
    }
private:
    std::shared_ptr<core::Context> context;
    std::shared_ptr<core::AbstractValue> active_node;
    core::ActionStack action_stack_;
private:
    boost::signals2::signal<void(std::shared_ptr<core::AbstractValue>)> changed_active_node_;
};

std::shared_ptr<EditorContext> global_dummy_context();

} // namespace studio

#endif
