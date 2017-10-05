/*  absttract_canvas.h - generic canvas which can be edited with tools
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

#ifndef STUDIO_CANVAS_ABSTRACT_CANVAS_H_4C76BD67_5064_5848_A03E_71D95507C8B2
#define STUDIO_CANVAS_ABSTRACT_CANVAS_H_4C76BD67_5064_5848_A03E_71D95507C8B2

#include <core/std/memory.h>
#include <core/std/vector.h>
#include <core/std/map.h>
#include <core/std/string.h>

#include <QGraphicsView>

#include <generic/context_listener.h>

class QGraphicsScene;
class QGraphicsItem;

namespace rainynite::studio {

class CanvasEditor;
class CanvasTool;

/**
 * Base class for canvas editable with tools.
 *
 * Register editors with ???
 * Register tools with ???
 *
 * You can subclass this to have a specific canvas with specific
 * tools and/or editors.
 */
class AbstractCanvas : public QGraphicsView, public ContextListener {
    Q_OBJECT

public:
    explicit AbstractCanvas(QWidget* parent = nullptr);
    virtual ~AbstractCanvas();

    /// Load registered tools for this canvas
    void load_registered_tools();

    /// Add new editor
    void add_editor(shared_ptr<CanvasEditor> editor);

    /// Remove editor
    void remove_editor(shared_ptr<CanvasEditor> editor);

    /// Remove all editors
    void clear_editors();

    /// Get available tool names list
    map<string, observer_ptr<CanvasTool>> const& list_tools() const;

    /// Switch to tool
    void use_tool(string name);

    /// Zoom at `point`
    void zoom_at(QPoint point, double factor);

    /// Set zoom level
    void set_zoom(double level);

    /// Zoom to show rect area
    void zoom_to_rect(QRectF rect);

    void scroll_by(QPoint delta);

    void set_context(shared_ptr<EditorContext> context) override;

Q_SIGNALS:
    void zoomed(double new_level);

protected:
    void active_node_changed(shared_ptr<core::AbstractValue> node) override;

    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

private:
    void add_tool(unique_ptr<CanvasTool> tool);
    void use_tool(observer_ptr<CanvasTool> tool);
    double zoom_level();

private:
    unique_ptr<QGraphicsScene> the_scene;
    vector<shared_ptr<CanvasEditor>> editors;
    vector<unique_ptr<CanvasTool>> tools;
    map<string, observer_ptr<CanvasTool>> named_tools;
    observer_ptr<CanvasTool> current_tool = nullptr;
    shared_ptr<core::AbstractValue> active_node;
};

} // namespace rainynite::studio

#endif
