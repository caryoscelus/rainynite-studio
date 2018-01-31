/*  point_editor.cpp - edit points on canvas
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

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QDebug>

#include <2geom/point.h>

#include <core/action_stack.h>
#include <core/actions/change_value.h>
#include <core/node_tree/transform.h>
#include <core/util/nullptr.h>

#include <generic/node_editor.h>
#include <generic/canvas_editor.h>
#include <generic/context_listener.h>
#include <widgets/canvas.h>
#include <util/geom.h>
#include "point_item.h"

namespace rainynite::studio {

/// Simple on-canvas point editor, based on PointItem.
class PointEditor :
    public NodeEditor,
    public CanvasEditor,
    public PointItemListener
{
public:
    virtual ~PointEditor() = default;

    void setup_canvas() override {
        point_item.reset(new ListenerPointItem(this));
        // GroupItem blocks movement, just Item is abstract, so using
        // an empty Pixmap item.
        item_group = make_unique<QGraphicsPixmapItem>();
        get_scene()->addItem(item_group.get());
        point_item->setParentItem(item_group.get());
        update_position();
    }
    void node_update() override {
        update_position();
    }
    void time_changed(core::Time time) override {
        ContextListener::time_changed(time);
        update_position();
    }

private:
    void update_position() {
        try {
            if (point_item == nullptr)
                return;
            auto node = get_node();
            if (auto maybe_point = get_value<Geom::Point>()) {
                no_null(node);
                auto point = *maybe_point;
                point_item->set_pos(point.x(), point.y());
                point_item->set_readonly(!node->can_set_any_at());
            }
            if (auto node_tree = get_context()->tree()) {
                if (auto calculate_tr = node_tree->get_element<core::TreeCalculateTransform>(get_node_index())) {
                    auto affine = calculate_tr->get_transform(get_core_context());
                    item_group->setTransform(QTransform{util::matrix(affine)});
                }
            }
        } catch (std::exception const& ex) {
            qWarning() << "Exception in PointEditor" << ex.what();
        }
    }

    void point_moved(size_t /*id*/, QPointF const& pos) override {
        if (auto node = get_node_as<Geom::Point>()) {
            if (node->can_set_any_at()) {
                auto action_stack = get_context()->action_stack();
                action_stack->emplace<core::actions::ChangeValueAt>(
                    node,
                    util::point(pos),
                    get_core_context()
                );
            }
        }
    }

    void point_stopped_moving(size_t /*id*/) override {
        if (auto action_stack = get_context()->action_stack())
            action_stack->close();
    }

private:
    observer_ptr<ListenerPointItem> point_item;
    unique_ptr<QGraphicsItem> item_group;
};


REGISTER_CANVAS_EDITOR(Canvas, PointEditor, Geom::Point);

} // namespace rainynite::studio
