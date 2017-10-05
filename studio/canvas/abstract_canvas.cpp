/*  abstract_canvas.cpp - generic canvas which can be edited with tools
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

#include <algorithm>

#include <QMouseEvent>

#include "abstract_canvas.h"
#include "registry.h"

namespace rainynite::studio {

AbstractCanvas::AbstractCanvas(QWidget* parent) :
    QGraphicsView(parent),
    the_scene(make_unique<QGraphicsScene>())
{
    setScene(the_scene.get());
}

void AbstractCanvas::load_registered_tools() {
    for (auto factory : get_canvas_tools_by_type(typeid(*this)))
        add_tool((*factory)());
}

void AbstractCanvas::add_tool(unique_ptr<CanvasTool> tool) {
    named_tools.emplace(tool->name(), tool.get());
    tools.push_back(std::move(tool));
}

AbstractCanvas::~AbstractCanvas() {
}

void AbstractCanvas::add_editor(shared_ptr<CanvasEditor> editor) {
    editors.push_back(editor);
    editor->set_canvas(this);
}

void AbstractCanvas::remove_editor(shared_ptr<CanvasEditor> editor) {
    editors.erase(std::remove(editors.begin(), editors.end(), editor), editors.end());
}

void AbstractCanvas::clear_editors() {
    editors.clear();
}

map<string, observer_ptr<CanvasTool>> const& AbstractCanvas::list_tools() const {
    return named_tools;
}

void AbstractCanvas::use_tool(string name) {
    auto it = named_tools.find(name);
    if (it == named_tools.end())
        return;
    use_tool(it->second);
}

void AbstractCanvas::use_tool(observer_ptr<CanvasTool> tool) {
    if (current_tool != nullptr) {
        removeEventFilter(current_tool.get());
        current_tool->set_canvas(nullptr);
    }
    tool->set_canvas(this);
    installEventFilter(tool.get());
    current_tool = tool;
}

void AbstractCanvas::zoom_at(QPoint point, double factor) {
    auto old_pos = mapToScene(point);
    scale(factor, factor);
    zoom_level *= factor;
    auto new_pos = mapToScene(point);
    auto delta = new_pos-old_pos;
    translate(delta.x(), delta.y());
    Q_EMIT zoomed(zoom_level);
}

void AbstractCanvas::set_zoom(double level) {
    zoom_at({width()/2, height()/2}, level/zoom_level);
}

void AbstractCanvas::scroll_by(QPoint delta) {
    delta /= transform().m11();
    translate(delta.x(), delta.y());
}

void AbstractCanvas::active_node_changed(shared_ptr<core::AbstractValue> node) {
    if (active_node != node) {
        // TODO: handle selection
        active_node = node;
        clear_editors();
        add_canvas_node_editor(*this, node);
    }
}

// Unfortunately, mouse events are not sent to filter..
#define EVENT_HANDLER(method, Event) \
void AbstractCanvas::method(Event* event) { \
    if (current_tool != nullptr) { \
        if (current_tool->eventFilter(this, event)) \
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
    for (auto const& editor : editors) {
        if (auto cl = dynamic_cast<ContextListener*>(editor.get())) {
            cl->set_context(context);
        }
    }
}

} // namespace rainynite::studio
