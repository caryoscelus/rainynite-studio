/*  bezier_editor.h - edit beziers on canvas
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

#ifndef STUDIO_CANVAS_BEZIER_EDITOR_H_301B627E_5F98_521F_93AD_92571F72CAAC
#define STUDIO_CANVAS_BEZIER_EDITOR_H_301B627E_5F98_521F_93AD_92571F72CAAC

#include <QPen>

#include <geom_helpers/knots.h>

#include <generic/node_editor.h>
#include <canvas/editor.h>
#include <generic/context_listener.h>

class QGraphicsItem;
class QGraphicsPathItem;

namespace rainynite::studio {

/// Primitive bezier path canvas editor
class BezierEditor : public NodeEditor, public CanvasEditor {
public:
    BezierEditor();
    virtual ~BezierEditor();

    void setup_canvas() override;
    void node_update() override;
    void time_changed(core::Time time_) override;

    void redraw();
    void init();
    void uninit();

    void set_display_tags(bool display_tags_);
    void set_curve_pen(QPen const& pen);

private:
    inline shared_ptr<core::BaseValue<Geom::BezierKnots>> get_bezier_node() {
        return dynamic_pointer_cast<core::BaseValue<Geom::BezierKnots>>(get_node());
    }
    inline Geom::BezierKnots get_path() {
        return get_bezier_node()->value(get_core_context());
    }

private:
    vector<unique_ptr<QGraphicsItem>> knot_items;
    unique_ptr<QGraphicsPathItem> curve_item;
    ssize_t old_size = -1;
    bool display_tags = true;
    QPen curve_pen;
};

} // namespace rainynite::studio

#endif
