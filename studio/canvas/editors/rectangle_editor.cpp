/*  rectangle_editor.cpp - edit rectangles on canvas
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

#include <QGraphicsItem>
#include <QGraphicsRectItem>

#include <geom_helpers/rectangle.h>

#include <core/node_tree_transform.h>

#include <generic/node_editor.h>
#include <generic/canvas_editor.h>
#include <widgets/canvas.h>
#include <util/geom.h>
#include <util/pen.h>

namespace rainynite::studio {

class RectangleEditor : public NodeEditor, public CanvasEditor {
public:
    virtual ~RectangleEditor() = default;

    void setup_canvas() override {
        rectangle_item.reset(get_scene()->addRect({}));
        rectangle_item->setPen(pens::cosmetic_dash());
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
        if (rectangle_item == nullptr)
            return;
        if (auto node = dynamic_cast<core::BaseValue<Geom::Rectangle>*>(get_node().get())) {
            auto rectangle = node->value(get_core_context());
            rectangle_item->setRect(
                rectangle.pos.x(),
                rectangle.pos.y(),
                rectangle.size.x(),
                rectangle.size.y()
            );
        }
        if (auto node_tree = get_context()->tree()) {
            if (auto calculate_tr = node_tree->get_element<core::TreeCalculateTransform>(get_node_index())) {
                auto affine = calculate_tr->get_transform(get_core_context());
                rectangle_item->setTransform(QTransform{util::matrix(affine)});
            }
        }
    }

    unique_ptr<QGraphicsRectItem> rectangle_item;
};

REGISTER_CANVAS_EDITOR(Canvas, RectangleEditor, Geom::Rectangle);

} // namespace rainynite::studio
