/*
 *  timeline_area.cpp - advanced timeline
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

#include <QGraphicsScene>

#include <generic/timeline_editor.h>

#include "timeline_area.h"

namespace rainynite::studio {

TimelineArea::TimelineArea(QWidget* parent) :
    QGraphicsView(parent),
    the_scene(std::make_unique<QGraphicsScene>())
{
    setScene(the_scene.get());
    setDragMode(QGraphicsView::RubberBandDrag);
    scale(16, 1);
}

TimelineArea::~TimelineArea() {
}

TimelineEditor* TimelineArea::add_editor(std::unique_ptr<TimelineEditor> editor) {
    auto editor_p = editor.get();
    editor->set_canvas(this);
    misc_editors.push_back(std::move(editor));
    return editor_p;
}

void TimelineArea::add_node_editor(std::shared_ptr<core::AbstractValue> node, std::unique_ptr<TimelineEditor> editor) {
    editor->set_canvas(this);
    node_editors[node] = std::move(editor);
}

void TimelineArea::set_context(std::shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    auto f = [&context](auto const& editor) {
        if (auto context_listener = dynamic_cast<ContextListener*>(editor.get()))
            context_listener->set_context(context);
    };
    for (auto const& editor : misc_editors)
        f(editor);
    for (auto const& p : node_editors)
        f(p.second);
}

void TimelineArea::clear_node_editors() {
    node_editors.clear();
}

} // namespace rainynite::studio
