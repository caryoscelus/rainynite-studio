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

#include <core/node/abstract_value.h>

#include <generic/canvas_editor.h>
#include "canvas.h"

using namespace fmt::literals;

namespace studio {

Canvas::Canvas(QWidget* parent) :
    QGraphicsView(parent),
    the_scene(std::make_unique<QGraphicsScene>()),
    image(std::make_unique<QGraphicsPixmapItem>())
{
    setScene(the_scene.get());
    the_scene->addItem(image.get());
}

Canvas::~Canvas() {
}

void Canvas::set_main_image(QPixmap const& pixmap) {
    image->setPixmap(pixmap);
}

void Canvas::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
    if (active_node != node) {
        active_node = node;
        clear_node_editors();
        add_canvas_editor(*this, node);
    }
}

void Canvas::add_node_editor(std::unique_ptr<CanvasEditor> editor) {
    editor->set_canvas(this);
    editors.push_back(std::move(editor));
}

void Canvas::remove_node_editor() {
    // TODO
    clear_node_editors();
}

void Canvas::clear_node_editors() {
    editors.clear();
}

} // namespace studio
