/*
 *  bezier_editor.h - edit beziers on canvas
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

#ifndef __STUDIO__WIDGETS__BEZIER_EDITOR_H__773FF04C
#define __STUDIO__WIDGETS__BEZIER_EDITOR_H__773FF04C

#include <memory>

#include <generic/node_editor.h>
#include <generic/canvas_editor.h>
#include <generic/context_listener.h>

class QGraphicsItem;

namespace studio {

class BezierKnotsDisplay : public NodeEditor, public CanvasEditor, public ContextListener {
public:
    BezierKnotsDisplay();
    virtual ~BezierKnotsDisplay();
public:
    virtual void set_canvas(Canvas* canvas) override;
    virtual void set_node(std::shared_ptr<core::AbstractValue> node) override;
    virtual void time_changed(core::Time time_) override;
private:
    void redraw();
    void init();
    void uninit();
private:
    std::vector<std::unique_ptr<QGraphicsItem>> knot_items;
};

} // namespace studio

#endif
