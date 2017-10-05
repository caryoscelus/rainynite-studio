/*  rectangle.cpp - draw rectangle tool
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

#include <QMouseEvent>
#include <QRubberBand>

#include <core/node/abstract_value.h>
#include <core/node/node.h>
#include <core/node/make.h>
#include <core/node_info.h>

#include <geom_helpers/rectangle.h>

#include <canvas/tool.h>
#include <widgets/canvas.h>

namespace rainynite::studio {
namespace tools {

/**
 * Rectangle draw tool.
 *
 * TODO: merge common code with zoom_area tool
 */
class Rectangle : public CanvasTool {
public:
    bool canvas_event(QEvent* event) override {
        if (get_canvas() == nullptr)
            return false;
        if (auto ctx = get_canvas()->get_context()) {
            if (auto node = ctx->get_active_node()) {
                if (accept(node)) {
                    active_node = std::move(node);
                    draw_rectangle(event);
                }
            }
        }
        return is_drawing;
    }

    string icon() const override {
        return "draw-rectangle";
    }

    string name() const override {
        return global_name();
    }

public:
    static string global_name() {
        return "Rectangle";
    }

protected:
    bool accept(shared_ptr<core::AbstractValue> node) {
        // TODO: accept nodes which may be rectangle or have rectangle children
        return node->get_type() == typeid(vector<Geom::Rectangle>);
//             || node->get_type() == typeid(Rectangle);
    }

    void rectangle_drawn(QPointF a, QPointF b) {
        if (auto node = dynamic_cast<core::BaseValue<vector<Geom::Rectangle>>*>(active_node.get())) {
            if (auto l = dynamic_cast<core::AbstractListLinked*>(node)) {
                auto rect_node = core::make_node_with_name<core::Node<Geom::Rectangle>>("RectangleAB");
                rect_node->set_property("a", core::make_value<Geom::Point>(a.x(), a.y()));
                rect_node->set_property("b", core::make_value<Geom::Point>(b.x(), b.y()));
                l->push_back(rect_node);
            }
        }
    }

private:
    void draw_rectangle(QEvent* event) {
        if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
            switch (mouse_event->type()) {
                case QEvent::MouseButtonPress: {
                    if (mouse_event->button() == Qt::LeftButton) {
                        rect_start = mouse_event->pos();
                        rubber_band = new QRubberBand(QRubberBand::Rectangle, get_canvas());
                        rubber_band->setGeometry({rect_start, rect_start});
                        rubber_band->show();
                        is_drawing = true;
                    }
                } break;
                case QEvent::MouseMove: {
                    if (is_drawing) {
                        rubber_band->setGeometry(get_rect(rect_start, mouse_event->pos()));
                    }
                } break;
                case QEvent::MouseButtonRelease: {
                    if (mouse_event->button() == Qt::LeftButton) {
                        auto rect = get_rect(rect_start, mouse_event->pos());
                        rubber_band->setGeometry(rect);
                        rectangle_drawn(
                            get_canvas()->mapToScene(rect_start),
                            get_canvas()->mapToScene(mouse_event->pos())
                        );
                        delete rubber_band;
                        is_drawing = false;
                    }
                } break;
                default:
                    break;
            }
        }
    }

    QRect get_rect(QPoint a, QPoint b) {
        return {
            std::min(a.x(), b.x()),
            std::min(a.y(), b.y()),
            std::abs(a.x()-b.x()),
            std::abs(a.y()-b.y())
        };
    }

private:
    bool is_drawing = false;
    QPoint rect_start;
    QRubberBand* rubber_band;
    shared_ptr<core::AbstractValue> active_node;
};

} // namespace tools

REGISTER_CANVAS_TOOL(tools::Rectangle, Canvas);

} // namespace rainynite::studio
