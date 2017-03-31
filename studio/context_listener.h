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

#include <core/context.h>

namespace studio {

class ContextListener {
public:
    ContextListener(std::shared_ptr<core::Context> context_=nullptr) :
        context(context_)
    {}
public:
    virtual std::shared_ptr<core::Context> get_context() const {
        return context;
    }
    virtual void set_context(std::shared_ptr<core::Context> context_) {
        context = context_;
    }
private:
    std::shared_ptr<core::Context> context;
};

}

#endif
