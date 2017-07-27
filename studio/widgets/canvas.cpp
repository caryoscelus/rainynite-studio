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

#include <cmath>

#include <fmt/format.h>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QDebug>

#include <geom_helpers/knots.h>

#include <core/node/abstract_value.h>
#include <core/document.h>

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
    setDragMode(QGraphicsView::RubberBandDrag);
    image_border.reset(the_scene->addRect(0, 0, 0, 0));
    setResizeAnchor(QGraphicsView::NoAnchor);
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

Canvas::~Canvas() {
}

void Canvas::wheelEvent(QWheelEvent* event) {
    auto old_pos = mapToScene(event->pos());
    auto scale_factor = std::pow(2, event->angleDelta().y() / 256.0);
    scale(scale_factor, scale_factor);
    auto new_pos = mapToScene(event->pos());
    auto delta = new_pos-old_pos;
    translate(delta.x(), delta.y());
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

void Canvas::set_context(std::shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    // TODO: listen to document change
    // TODO: listen to size node change
    // TODO: possible *nullptr
    connect_boost(
        get_core_context()->get_document()->get_size()->changed_signal,
        [this]() {
            update_border();
        }
    );
    update_border();
}

void Canvas::update_border() {
    auto size = get_core_context()->get_document()->get_size()->get(get_time());
    image_border->setRect(0, 0, size.x(), size.y());
}

} // namespace studio
