/*
 *  context_listener.cpp - Context-dependent entity
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

#include <core/context.h>

#include "context_listener.h"

namespace studio {

ContextListener::ContextListener(std::shared_ptr<core::Context> context_) :
    context(context_),
    destroy_detector(std::make_shared<Null>())
{}

std::shared_ptr<core::Context> ContextListener::get_context() const {
    return context;
}

void ContextListener::set_context(std::shared_ptr<core::Context> context_) {
    context = context_;
    if (auto context = get_context()) {
        connect_boost(
            context->changed_time,
            [this](core::Time time) {
                time_changed(time);
            }
        );
        connect_boost(
            context->changed_active_node,
            [this](std::shared_ptr<core::AbstractValue> node) {
                active_node_changed(node);
            }
        );
    }
}

}
