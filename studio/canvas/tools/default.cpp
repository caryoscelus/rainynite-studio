/*  default.cpp - default tool
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

#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>

#include <canvas/tool.h>
#include <widgets/canvas.h>

namespace rainynite::studio {

class DefaultTool : public CanvasTool {
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
            if (is_scrolling) {
                get_canvas()->scroll_by(mouse_event->pos() - scroll_pos);
                scroll_pos = mouse_event->pos();
            }
            switch (mouse_event->type()) {
                case QEvent::MouseButtonPress: {
                    if (mouse_event->button() == Qt::MidButton) {
                        scroll_pos = mouse_event->pos();
                        is_scrolling = true;
                    }
                } break;
                case QEvent::MouseButtonRelease: {
                    is_scrolling = false;
                } break;
                default:
                    break;
            }
        }
        return is_scrolling;
    }

public:
    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Default";
    }

private:
    bool is_scrolling = false;
    QPoint scroll_pos;
};

REGISTER_CANVAS_TOOL(DefaultTool, Canvas);

} // namespace rainynite::studio
