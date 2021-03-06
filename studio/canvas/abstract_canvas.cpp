/*  abstract_canvas.cpp - generic canvas which can be edited with tools
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

#include <algorithm>

#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QDebug>

#include <util/strings.h>
#include "abstract_canvas.h"
#include "registry.h"

namespace rainynite::studio {

AbstractCanvas::AbstractCanvas(QWidget* parent) :
    QGraphicsView(parent),
    the_scene(make_unique<QGraphicsScene>()),
    image(make_unique<QGraphicsPixmapItem>())
{
    setScene(the_scene.get());
    scene()->addItem(image.get());
}

AbstractCanvas::~AbstractCanvas() = default;

void AbstractCanvas::set_background_image(QPixmap const& pixmap) {
    image->setPixmap(pixmap);
}

void AbstractCanvas::set_bg_transform(QTransform const& transform) {
    image->setTransform(transform);
}

void AbstractCanvas::load_registered_tools() {
    for (auto factory : get_canvas_tools_by_type(typeid(*this)))
        add_tool((*factory)());
}

void AbstractCanvas::add_tool(unique_ptr<CanvasTool> tool) {
    named_tools.emplace(tool->name(), tool.get());
    tools.push_back(std::move(tool));
}

void AbstractCanvas::add_editor(shared_ptr<AbstractCanvasEditor> editor) {
    editors.push_back(editor);
    editor->set_canvas(this);
}

void AbstractCanvas::remove_editor(shared_ptr<AbstractCanvasEditor> editor) {
    editors.erase(std::remove(editors.begin(), editors.end(), editor), editors.end());
}

void AbstractCanvas::clear_editors() {
    editors.clear();
}

shared_ptr<AbstractCanvasEditor> AbstractCanvas::latest_editor() const {
    if (editors.size() > 0)
        return editors.back();
    return nullptr;
}

vector<observer_ptr<CanvasTool>> AbstractCanvas::list_tools() const {
    vector<observer_ptr<CanvasTool>> result;
    std::transform(
        tools.begin(),
        tools.end(),
        std::back_inserter(result),
        [](auto const& e) {
            return make_observer(e.get());
        }
    );
    return result;
}

void AbstractCanvas::use_tool(string const& name) {
    auto it = named_tools.find(name);
    if (it == named_tools.end()) {
        qWarning() << "Cannot find tool" << util::str(name);
        return;
    }
    if (use_tool(it->second))
        Q_EMIT tool_changed(name);
}

bool AbstractCanvas::use_tool(observer_ptr<CanvasTool> tool) {
    if (current_tool == tool)
        return false;
    if (current_tool != nullptr) {
        removeEventFilter(current_tool.get());
        current_tool->set_canvas(nullptr);
    }
    tool->set_canvas(this);
    installEventFilter(tool.get());
    current_tool = tool;
    return true;
}

void AbstractCanvas::zoom_at(QPoint point, double factor) {
    auto old_pos = mapToScene(point);
    scale(factor, factor);
    auto new_pos = mapToScene(point);
    auto delta = new_pos-old_pos;
    translate(delta.x(), delta.y());
    Q_EMIT zoomed();
}

void AbstractCanvas::set_zoom(double level) {
    zoom_at({width()/2, height()/2}, level/zoom_level());
}

void AbstractCanvas::zoom_to_rect(QRectF rect) {
    fitInView(rect, Qt::KeepAspectRatio);
    Q_EMIT zoomed();
}

double AbstractCanvas::zoom_level() {
    return std::abs(transform().m11());
}

void AbstractCanvas::mirror_horizontally(bool value) {
    if (value != is_mirrored_horizontally())
        scale(-1.0, 1.0);
}

bool AbstractCanvas::is_mirrored_horizontally() const {
    return transform().m11() < 0;
}

void AbstractCanvas::scroll_by(QPointF delta) {
    delta /= transform().m11();
    translate(delta.x(), delta.y());
}

// Unfortunately, mouse events are not sent to filter..
#define EVENT_HANDLER(method, Event) \
void AbstractCanvas::method(Event* event) { \
    if (current_tool != nullptr) { \
        if (current_tool->eventFilter(this, event)) \
            return; \
    } \
    if (auto editor = latest_editor()) { \
        if (editor->canvas_event(event)) \
            return; \
    } \
    QGraphicsView::method(event); \
}
#define MOUSE_HANDLER(method) EVENT_HANDLER(method, QMouseEvent)

MOUSE_HANDLER(mouseDoubleClickEvent)
MOUSE_HANDLER(mouseMoveEvent)
MOUSE_HANDLER(mousePressEvent)
MOUSE_HANDLER(mouseReleaseEvent)

EVENT_HANDLER(wheelEvent, QWheelEvent)

#undef MOUSE_HANDLER
#undef EVENT_HANDLER

void AbstractCanvas::set_context(shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    for (auto const& editor : editors) {
        if (auto cl = dynamic_cast<ContextListener*>(editor.get())) {
            cl->set_context(context);
        }
    }
}

} // namespace rainynite::studio
