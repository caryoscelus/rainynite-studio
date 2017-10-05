/*  base.h - base for tools that support zooming & panning
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

#ifndef STUDIO_CANVAS_TOOLS_BASE_H_BC869E8B_6486_5669_8027_CE9D9EDF4C90
#define STUDIO_CANVAS_TOOLS_BASE_H_BC869E8B_6486_5669_8027_CE9D9EDF4C90

#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>

#include <canvas/tool.h>
#include <widgets/canvas.h>

namespace rainynite::studio::tools {

class Base : public CanvasTool {
public:
    bool canvas_event(QEvent* event) override {
        if (get_canvas() == nullptr)
            return false;
        if (auto wheel_event = dynamic_cast<QWheelEvent*>(event)) {
            auto scale_factor = std::pow(2, wheel_event->angleDelta().y() / 256.0);
            get_canvas()->zoom_at(wheel_event->pos(), scale_factor);
            return true;
        }
        if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
            auto delta = mouse_event->pos() - scroll_pos;
            scroll_pos = mouse_event->pos();
            if (is_scrolling) {
                get_canvas()->scroll_by(delta);
            } else if (is_zooming) {
                auto scale_factor = std::pow(2, -delta.y() / 64.0);
                get_canvas()->zoom_at(zoom_pos, scale_factor);
            }
            switch (mouse_event->type()) {
                case QEvent::MouseButtonPress: {
                    if (is_scrolling || is_zooming) {
                        // this shouldn't really happen, but just in case
                        break;
                    }
                    if (mouse_event->button() == Qt::MidButton) {
                        if (mouse_event->modifiers().testFlag(Qt::ControlModifier)) {
                            is_zooming = true;
                            zoom_pos = mouse_event->pos();
                        } else {
                            is_scrolling = true;
                        }
                    }
                } break;
                case QEvent::MouseButtonRelease: {
                    if (mouse_event->button() == Qt::MidButton)
                        is_zooming = is_scrolling = false;
                } break;
                default:
                    break;
            }
        }
        return is_scrolling || is_zooming;
    }

private:
    bool is_scrolling = false;
    bool is_zooming = false;
    QPoint scroll_pos;
    QPoint zoom_pos;
};

} // namespace rainynite::studio::tools

#endif
