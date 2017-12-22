/*  canvas.cpp - main canvas widget
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
#include <QGraphicsRectItem>
#include <QWheelEvent>
#include <QSlider>
#include <QDebug>

#include <geom_helpers/knots.h>

#include <core/node/abstract_value.h>
#include <core/document.h>

#include <generic/canvas_editor.h>
#include <generic/node_editor.h>
#include <util/pen.h>
#include "canvas.h"

using namespace fmt::literals;

namespace rainynite::studio {

static const int SLIDER_FACTOR = 8;
static const int MIN_ZOOM = 6; // x64
static const int MAX_ZOOM = 8; // x256

Canvas::Canvas(QWidget* parent) :
    AbstractCanvas(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    image_border.reset(scene()->addRect(0, 0, 0, 0));
    image_border->setPen(pens::cosmetic_dash());
    setResizeAnchor(QGraphicsView::NoAnchor);
    setTransformationAnchor(QGraphicsView::NoAnchor);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    zoom_slider.reset(new QSlider(Qt::Horizontal, this));
    zoom_slider->setRange(-SLIDER_FACTOR*MIN_ZOOM, SLIDER_FACTOR*MAX_ZOOM);
    connect(
        zoom_slider.get(),
        &QSlider::valueChanged,
        [this] (int value) {
            set_zoom(std::pow(2, value*1.0/SLIDER_FACTOR));
        }
    );
    connect(
        this,
        &AbstractCanvas::zoomed,
        [this]() {
            auto old_lock = zoom_slider->blockSignals(true);
            zoom_slider->setValue(std::log2(zoom_level())*SLIDER_FACTOR);
            zoom_slider->blockSignals(old_lock);
        }
    );
}

Canvas::~Canvas() {
}

void Canvas::add_editor(shared_ptr<AbstractCanvasEditor> editor) {
    AbstractCanvas::add_editor(editor);
    if (auto canvas_editor = dynamic_pointer_cast<CanvasEditor>(std::move(editor))) {
        if (auto node_editor = dynamic_pointer_cast<NodeEditor>(canvas_editor)) {
            canvas_editor->set_transform({});
        }
    }
}

void Canvas::set_context(shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    // TODO: listen to document change
    // TODO: listen to size node change
    // TODO: possible *nullptr
    get_core_context()->get_document()->get_size()->subscribe(
        [this]() {
            update_border();
        }
    );
    update_border();
}

void Canvas::resizeEvent(QResizeEvent* event) {
    auto pos = event->size();
    zoom_slider->setGeometry({
        pos.width()-128,
        pos.height()-16,
        128,
        16
    });
}

void Canvas::update_border() {
    auto size = get_core_context()->get_document()->get_size()->value(get_core_context());
    auto w = size.x();
    auto h = size.y();
    image_border->setRect(0, 0, w, h);
    setSceneRect(-w, -h, w*3, h*3);
}

} // namespace rainynite::studio
