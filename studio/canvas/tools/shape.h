/*  shape.h - base for shape creating tools
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

#ifndef STUDIO_CANVAS_TOOLS_SHAPE_H_5A163439_B575_5309_B679_7539C7DF5AB7
#define STUDIO_CANVAS_TOOLS_SHAPE_H_5A163439_B575_5309_B679_7539C7DF5AB7

#include <2geom/point.h>

#include "base.h"

namespace rainynite::studio::tools {

/**
 * Base class for shape-creating tools.
 */
class Shape : public Base {
public:
    bool canvas_event(QEvent* event) override;

protected:
    using MakeShape = std::function<shared_ptr<core::AbstractValue>(Geom::Point)>;

    void new_shape_at(QPointF const& pos, MakeShape f);

    virtual bool draw_shape_event(QEvent* event);

    virtual bool mouse_press(QPoint const& pos) = 0;
    virtual bool mouse_move(QPoint const& /*pos*/) {
        return false;
    }
    virtual bool mouse_release(QPoint const& /*pos*/) {
        return false;
    }

    /**
     * Called on double click which should indicate editing is finished.
     *
     * Default implementation is empty because some shapes are finished
     * implicitly by end of dragging (circle, rectangle, etc)
     */
    virtual bool editing_done() {
        return false;
    }

    /// Returns whether this node can accept the shape
    virtual bool accept(shared_ptr<core::AbstractValue> node);

    /// Write shape to active node
    virtual void write_shape(shared_ptr<core::AbstractValue> node);

    /// Get target node index
    core::NodeTree::Index get_index() const {
        return target_node_index;
    }

private:
    void add_renderable_to_list(shared_ptr<core::ActionStack> action_stack, shared_ptr<core::AbstractListLinked> list, shared_ptr<core::AbstractValue> shape);

private:
    core::NodeTree::Index target_node_index;
};

} // namespace rainynite::studio::tools

#endif
