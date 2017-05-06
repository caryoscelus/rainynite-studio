/*
 *  canvas.cpp - main canvas widget
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

#include <fmt/format.h>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>

#include <geom_helpers/knots.h>

#include <core/node.h>

#include "canvas.h"

using namespace fmt::literals;

namespace studio {

Canvas::Canvas(QWidget* parent) :
    QGraphicsView(parent),
    scene(std::make_unique<QGraphicsScene>()),
    image(std::make_unique<QGraphicsPixmapItem>())
{
    setScene(scene.get());
    scene->addItem(image.get());
}

Canvas::~Canvas() {
}

void Canvas::set_main_image(QPixmap const& pixmap) {
    image->setPixmap(pixmap);
}

void Canvas::time_changed(core::Time) {
    redraw_selected_node();
}

void Canvas::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
    active_node = node;
    redraw_selected_node();
}

void Canvas::redraw_selected_node() {
    for (auto const& e : knot_items) {
        scene->removeItem(e.get());
    }
    knot_items.clear();
    if (auto bezier_node = dynamic_cast<core::BaseValue<Geom::BezierKnots>*>(active_node.get())) {
        Geom::BezierKnots path;
        try {
            path = bezier_node->get(get_core_context()->get_time());
        } catch (std::exception const& ex) {
            qDebug() << QString::fromStdString("Uncaught exception while getting path: {}"_format(ex.what()));
            return;
        }
        for (auto const& knot : path.knots) {
            auto x = knot.pos.x();
            auto y = knot.pos.y();
            auto e = scene->addEllipse(x-2, y-2, 4, 4);
            knot_items.emplace_back(e);
            if (!knot.uid.empty()) {
                auto e = scene->addText(QString::fromStdString(knot.uid));
                e->setX(knot.pos.x());
                e->setY(knot.pos.y());
                knot_items.emplace_back(e);
            }
        }
    }
}

} // namespace studio
