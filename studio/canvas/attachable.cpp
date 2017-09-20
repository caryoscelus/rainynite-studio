/*  attachable.cpp - attachable to canvas interface
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

#include "attachable.h"
#include "abstract_canvas.h"

namespace rainynite::studio {

void CanvasAttachable::set_canvas(AbstractCanvas* canvas_) {
    if (canvas != nullptr)
        detach_canvas();
    canvas = canvas_;
    if (canvas != nullptr)
        setup_canvas();
}

QGraphicsScene* CanvasAttachable::get_scene() const {
    if (canvas)
        return canvas->scene();
    return nullptr;
}

} // namespace rainynite::studio
