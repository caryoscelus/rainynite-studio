/*  time_bezier.cpp - bezier editor for timearea
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

#include <generic/node_editor.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include <util/pen.h>
#include "bezier_editor.h"

namespace rainynite::studio {

class TimeBezierEditor :
    public TimelineEditor,
    public NodeEditor
{
public:
    TimeBezierEditor() :
        editor(make_unique<BezierEditor>())
    {
        editor->set_display_tags(false);
        editor->set_curve_pen(pens::cosmetic_thick_light_solid());
    }

    void setup_canvas() override {
        editor->set_canvas(get_canvas());
    }
    void detach_canvas() override {
        editor->set_canvas(nullptr);
    }
    void node_update() override {
        editor->set_node(get_node());
    }

    void set_context(shared_ptr<EditorContext> context) override {
        ContextListener::set_context(context);
        editor->set_context(context);
    }

private:
    unique_ptr<BezierEditor> const editor;
};

REGISTER_CANVAS_EDITOR(TimelineArea, TimeBezierEditor, Geom::BezierKnots);

}
