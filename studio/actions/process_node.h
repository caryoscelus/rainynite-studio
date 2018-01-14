/*  process_node.h - node processor
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

#ifndef STUDIO_ACTIONS_PROCESS_NODE_H_D9E2969B_5C37_5085_828C_4708ED7EC469
#define STUDIO_ACTIONS_PROCESS_NODE_H_D9E2969B_5C37_5085_828C_4708ED7EC469

#include <core/util/class_init.h>
#include <core/abstract_factory.h>

#include <generic/context_listener.h>

namespace rainynite::studio::actions {

template <typename T, typename Tag>
class ProcessNode : public ContextListener {
public:
    virtual bool accept(core::NodeTree::Index index) const = 0;
    virtual void feed(T const& argument) = 0;
    virtual void start_list() {}
    virtual void end_list() {}

    bool set_node(core::NodeTree::Index index_) {
        if (accept(index_)) {
            index = index_;
            return true;
        }
        return false;
    }

    core::NodeTree::Index get_node_index() const {
        return index;
    }

    shared_ptr<core::AbstractValue> get_node() const {
        return get_context()->tree()->get_node(index);
    }

private:
    core::NodeTree::Index index;
};

template <class P, typename T, typename Tag>
struct ProcessNodeFactoryImpl :
    public AbstractFactoryImpl<ProcessNode<T, Tag>, P>,
    private class_init::ListRegistered<
        ProcessNodeFactoryImpl<P, T, Tag>,
        AbstractFactory<ProcessNode<T, Tag>>
    >
{};

#define REGISTER_PROCESS_NODE(Processor, Type, Tag) \
template struct ProcessNodeFactoryImpl<Processor, Type, Tag>;

} // namespace rainynite::studio::actions

#endif
