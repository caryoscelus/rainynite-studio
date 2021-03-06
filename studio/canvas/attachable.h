/*  attachable.h - attachable to canvas interface
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

#ifndef STUDIO_CANVAS_ATTACHABLE_H_3B57A84F_8276_545C_8A78_11812A0ECA88
#define STUDIO_CANVAS_ATTACHABLE_H_3B57A84F_8276_545C_8A78_11812A0ECA88

class QGraphicsScene;
class QEvent;

namespace rainynite::studio {

class AbstractCanvas;

class CanvasAttachable {
public:
    virtual ~CanvasAttachable() = default;

    void set_canvas(AbstractCanvas* canvas_);

    /// Called by canvas on mouse event
    virtual bool canvas_event(QEvent* /*event*/) {
        return false;
    }

protected:
    virtual void detach_canvas() {}
    virtual void setup_canvas() {}

    AbstractCanvas* get_canvas() const {
        return canvas;
    }

    QGraphicsScene* get_scene() const;

private:
    AbstractCanvas* canvas = nullptr;
};

} // namespace rainynite::studio

#endif
