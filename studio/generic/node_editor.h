/*
 *  node_editor.h - abstract node editor widget class
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

#ifndef __STUDIO__NODE_EDITOR_H__8BFEEC92
#define __STUDIO__NODE_EDITOR_H__8BFEEC92

#include <memory>

namespace core {
class AbstractValue;
}

namespace studio {

class NodeEditor {
public:
    virtual void set_node(std::shared_ptr<core::AbstractValue> node_);
    inline std::shared_ptr<core::AbstractValue> get_node() {
        return node;
    }
private:
    std::shared_ptr<core::AbstractValue> node = nullptr;
};

} // namespace studio

#endif
