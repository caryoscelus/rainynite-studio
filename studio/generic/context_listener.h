/*
 *  context_listener.h - Context-dependent entity
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

#ifndef __STUDIO__CONTEXT_LISTENER_H__9E128DC4
#define __STUDIO__CONTEXT_LISTENER_H__9E128DC4

#include <memory>
#include <type_traits>

#include "editor_context.h"

namespace studio {

class ContextListener {
public:
    ContextListener(std::shared_ptr<EditorContext> context_=nullptr);
    virtual ~ContextListener() = default;

public:
    std::shared_ptr<core::Context> get_core_context() const {
        return context->get_context();
    }
    std::shared_ptr<EditorContext> get_context() const {
        return context;
    }
    void set_core_context(std::shared_ptr<core::Context> core_context) {
        set_context(std::make_shared<EditorContext>(core_context));
    }
    virtual void set_context(std::shared_ptr<EditorContext> context_);

protected:
    virtual core::Time get_time() {
        return time;
    }
    virtual void time_changed(core::Time time_) {
        time = time_;
    }
    virtual void fps_changed(core::Time::fps_type) {}
    virtual void active_node_changed(std::shared_ptr<core::AbstractValue>) {}

    template <class S, class F>
    void connect_boost(S& signal, F lambda) {
        auto slot = typename S::slot_type(lambda);
        slot.track_foreign(destroy_detector);
        signal.connect(slot);
    }

private:
    std::shared_ptr<EditorContext> context;
    core::Time time;
    struct Null {};
    std::shared_ptr<Null> destroy_detector;
};

}

#endif
