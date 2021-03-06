/*  timeline_editor.h - timeline editing widgets
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

#ifndef STUDIO_GENERIC_TIMELINE_EDITOR_H_805B2CB8_423E_5B86_BC21_F2CA9ABFA520
#define STUDIO_GENERIC_TIMELINE_EDITOR_H_805B2CB8_423E_5B86_BC21_F2CA9ABFA520

#include <core/std/memory.h>
#include <core/util/class_init.h>

#include <canvas/abstract_editor.h>

class QMenu;

namespace rainynite::studio {

/**
 * Timeline editor: specialization of AbstractCanvasEditor for timeline.
 */
class TimelineEditor : public AbstractCanvasEditor {
public:
    /// Set editor preferred vertical position and size
    virtual void set_position_hint(int y, int height) {
        editor_y = y;
        editor_height = height;
    }

    bool call_context_menu(double y, double seconds, QMenu& menu);

    /// Context menu
    virtual bool context_menu(double /*seconds*/, QMenu& /*menu*/) {
        return false;
    }

protected:
    int editor_y = 0;
    int editor_height = 0;
};

} // namespace rainynite::studio

#endif
