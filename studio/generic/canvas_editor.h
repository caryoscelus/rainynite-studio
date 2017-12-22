/*  canvas_editor.h - canvas editor
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

#ifndef GENERIC_CANVAS_EDITOR_H_2A8A29CC_A76A_5226_BA88_A39F61B5F43E
#define GENERIC_CANVAS_EDITOR_H_2A8A29CC_A76A_5226_BA88_A39F61B5F43E

#include <core/std/memory.h>
#include <core/class_init.h>

#include <canvas/abstract_editor.h>

class QTransform;

namespace rainynite::studio {

class CanvasEditor : public AbstractCanvasEditor {
public:
    /// Set editor base transform
    virtual void set_transform(QTransform const& /*transform*/) {}
};

} // namespace rainynite::studio

#endif
