/*  timeline_area.h - advanced timeline
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

#ifndef STUDIO_WIDGETS_TIMELINE_AREA_H_642C9BBC_A699_5EA7_99D3_EDF15CA95ECD
#define STUDIO_WIDGETS_TIMELINE_AREA_H_642C9BBC_A699_5EA7_99D3_EDF15CA95ECD

#include <QGraphicsView>

#include <core/std/memory.h>
#include <core/std/map.h>
#include <core/std/vector.h>

#include <canvas/abstract_canvas.h>
#include <canvas/registry.h>

class QGraphicsScene;
class QMenu;

namespace rainynite::studio {

class TimelineEditor;

class TimelineArea : public AbstractCanvas {
public:
    explicit TimelineArea(QWidget* parent = nullptr);
    virtual ~TimelineArea();

    void contextMenuEvent(QContextMenuEvent* event) override;

    void zoom_time_by(double factor);

    void add_misc_editor(shared_ptr<AbstractCanvasEditor> editor);

private:
    vector<shared_ptr<AbstractCanvasEditor>> misc_editors;
    unique_ptr<QMenu> context_menu;
};

REGISTER_CANVAS(TimelineArea);

} // namespace rainynite::studio

#endif
