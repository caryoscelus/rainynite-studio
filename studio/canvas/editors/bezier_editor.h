/*  bezier_editor.h - edit beziers on canvas
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

#ifndef STUDIO_CANVAS_BEZIER_EDITOR_H_301B627E_5F98_521F_93AD_92571F72CAAC
#define STUDIO_CANVAS_BEZIER_EDITOR_H_301B627E_5F98_521F_93AD_92571F72CAAC

#include <QPen>

#include <geom_helpers/knots.h>

#include <generic/node_editor.h>
#include <generic/canvas_editor.h>
#include <generic/context_listener.h>
#include "point_item.h"

class QGraphicsItem;
class QGraphicsPathItem;

namespace rainynite::studio {

/**
 * On-canvas bezier path editor
 */
class BezierEditor :
    public NodeEditor,
    public CanvasEditor,
    public PointItemListener
{
public:
    BezierEditor();
    virtual ~BezierEditor();

    void setup_canvas() override;
    void node_update() override;
    void time_changed(core::Time time_) override;

    void redraw();
    void init();
    void uninit();

    void set_appending(bool value) {
        appending = value;
    }

    void set_display_tags(bool display_tags_);
    void set_curve_pen(QPen const& pen);

    bool canvas_event(QEvent* event) override;

    void point_moved(size_t point_id, QPointF const& pos) override;
    void point_stopped_moving(size_t point_id) override;
    void point_double_clicked(size_t point_id) override;

    bool is_readonly() const;

    enum BezierPointFlag {
        Point = 0,
        Tg1,
        Tg2,
        Count
    };

private:
    void add_knot_editor(size_t i);

    void reset_curve(Geom::BezierKnots const& path);

    QGraphicsItem* add_point_editor(size_t i, BezierPointFlag what, QGraphicsItem* parent = nullptr);

    void add_tags();
    void remove_tags();

    shared_ptr<core::BaseValue<Geom::BezierKnots>> get_bezier_node() const;
    Geom::BezierKnots get_path() const;

    Geom::Point convert_pos(QPoint const& src) const;

    pair<BezierPointFlag, size_t> point_id_to_fn(size_t src) const;
    size_t point_id_from_fn(BezierPointFlag flag, size_t n) const;

private:
    struct EventFilter;
    unique_ptr<EventFilter> event_filter;

    vector<unique_ptr<QGraphicsItem>> knot_items;
    vector<unique_ptr<QGraphicsItem>> tag_items;
    unique_ptr<QGraphicsPathItem> curve_item;
    ptrdiff_t old_size = -1;
    bool display_tags = true;
    QPen curve_pen;
    bool appending = false;
    bool drawing = false;
};

} // namespace rainynite::studio

#endif
