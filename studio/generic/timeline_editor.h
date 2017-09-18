/*
 *  timeline_editor.h - timeline editing widgets
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

#ifndef __STUDIO__GENERIC__TIMELINE_EDITOR_H__043F26E4
#define __STUDIO__GENERIC__TIMELINE_EDITOR_H__043F26E4

#include <core/std/memory.h>
#include <core/class_init.h>

#include <canvas/editor.h>

namespace rainynite::studio {

class TimelineEditor : public CanvasEditor {
public:
    /// Set editor preferred vertical position and size
    virtual void set_position_hint(int /*y*/, int /*height*/) {
    }
};

} // namespace rainynite::studio

#endif
