/*  context_listener.h - Context-dependent entity
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

#ifndef STUDIO_GENERIC_CONTEXT_LISTENER_H_4EE76790_980D_55BC_9584_1B71A700816E
#define STUDIO_GENERIC_CONTEXT_LISTENER_H_4EE76790_980D_55BC_9584_1B71A700816E

#include <core/std/memory.h>
#include <type_traits>

#include "editor_context.h"
#include "destroy_detector.h"

namespace rainynite::studio {

class ContextListener : public DestroyDetector {
public:
    ContextListener(shared_ptr<EditorContext> context_=nullptr);
    virtual ~ContextListener() = default;

public:
    shared_ptr<core::Context> get_core_context() const {
        return context->get_context();
    }
    shared_ptr<EditorContext> get_context() const {
        return context;
    }
    void set_core_context(shared_ptr<core::Context> core_context) {
        set_context(make_shared<EditorContext>(core_context));
    }
    virtual void set_context(shared_ptr<EditorContext> context_);

protected:
    core::Time get_time() const {
        return time;
    }
    virtual void time_changed(core::Time time_) {
        time = time_;
    }
    virtual void fps_changed(core::Time::fps_type) {}
    virtual void active_node_changed(shared_ptr<core::AbstractValue>) {}

private:
    shared_ptr<EditorContext> context;
    core::Time time;
};

}

#endif
