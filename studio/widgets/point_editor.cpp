/*
 *  point_editor.cpp - edit points on canvas
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
#include <QGraphicsEllipseItem>

#include <2geom/point.h>

#include <widgets/canvas.h>
#include "point_editor.h"

namespace studio {

class PointItem : public QGraphicsEllipseItem {
public:
    using Callback = std::function<void(double, double)>;
public:
    static const int radius = 2;
public:
    PointItem(Callback callback) :
        QGraphicsEllipseItem(0, 0, radius*2, radius*2),
        position_callback(callback)
    {}
public:
    virtual QVariant itemChange(GraphicsItemChange change, QVariant const& value) override {
        if (change == ItemPositionHasChanged) {
            auto pos = value.toPointF();
            position_callback(pos.x()+radius, pos.y()+radius);
        }
        return QGraphicsItem::itemChange(change, value);
    }
    void set_readonly(bool ro) {
        setFlag(QGraphicsItem::ItemIsMovable, !ro);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, !ro);
    }
    void set_pos(double x, double y) {
        setPos({x-radius, y-radius});
    }
private:
    Callback position_callback;
};

PointEditor::PointEditor() {
}

PointEditor::~PointEditor() {
}

void PointEditor::set_canvas(Canvas* canvas) {
    CanvasEditor::set_canvas(canvas);
    point_item = std::make_unique<PointItem>(
        [this](double x, double y) {
            save_position(x, y);
        }
    );
    canvas->scene()->addItem(point_item.get());
    update_position();
}

void PointEditor::set_node(std::shared_ptr<core::AbstractValue> node) {
    NodeEditor::set_node(node);
    update_position();
}

void PointEditor::time_changed(core::Time) {
    update_position();
}

void PointEditor::update_position() {
    if (point_item == nullptr)
        return;
    if (auto node = dynamic_cast<core::BaseValue<Geom::Point>*>(get_node().get())) {
        auto point = node->get(get_time());
        point_item->set_pos(point.x(), point.y());
        point_item->set_readonly(!node->can_set());
    }
}

void PointEditor::save_position(double x, double y) {
    if (auto node = dynamic_cast<core::BaseValue<Geom::Point>*>(get_node().get())) {
        if (node->can_set())
            node->set({x, y});
    }
}

} // namespace studio
