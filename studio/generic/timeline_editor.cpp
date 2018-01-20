/*  timeline_editor.cpp - timeline editing widgets
 *  Copyright (C) 2018 caryoscelus
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

#include <canvas/abstract_canvas.h>
#include "timeline_editor.h"

namespace rainynite::studio {

bool TimelineEditor::call_context_menu(double y, double seconds, QMenu& menu) {
    if (editor_y <= y && y < editor_y+editor_height) {
        return context_menu(seconds, menu);
    }
    return false;
}

} // namespace rainynite::studio
