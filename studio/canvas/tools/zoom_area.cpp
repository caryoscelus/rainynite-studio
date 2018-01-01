/*  zoom_area.cpp - zoom area tool
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

#include <QRubberBand>

#include "base.h"

namespace rainynite::studio::tools {

class ZoomArea : public Base {
public:
    bool canvas_event(QEvent* event) override {
        if (get_canvas() == nullptr)
            return false;
        if (auto mouse_event = dynamic_cast<QMouseEvent*>(event)) {
            switch (mouse_event->type()) {
                case QEvent::MouseButtonPress: {
                    if (mouse_event->button() == Qt::MidButton) {
                        break;
                    }
                    zooming_button = mouse_event->button();
                    zoom_start = mouse_event->pos();
                    rubber_band = new QRubberBand(QRubberBand::Rectangle, get_canvas());
                    rubber_band->setGeometry({zoom_start, zoom_start});
                    rubber_band->show();
                    is_zooming = true;
                } break;
                case QEvent::MouseMove: {
                    if (is_zooming) {
                        rubber_band->setGeometry(get_rect(zoom_start, mouse_event->pos()));
                    }
                } break;
                case QEvent::MouseButtonRelease: {
                    if (mouse_event->button() == zooming_button) {
                        auto rect = get_rect(zoom_start, mouse_event->pos());
                        rubber_band->setGeometry(rect);
                        auto scene_rect = QRectF {
                            get_canvas()->mapToScene(rect.topLeft()),
                            get_canvas()->mapToScene(rect.bottomRight())
                        };
                        get_canvas()->zoom_to_rect(scene_rect);
                        delete rubber_band;
                        is_zooming = false;
                    }
                } break;
                default:
                    break;
            }
        }
        return is_zooming || Base::canvas_event(event);
    }

    string icon() const override {
        return "zoom-select";
    }

    string name() const override {
        return global_name();
    }

public:
    static string global_name() {
        return "Zoom area";
    }

private:
    QRect get_rect(QPoint a, QPoint b) {
        return {
            std::min(a.x(), b.x()),
            std::min(a.y(), b.y()),
            std::abs(a.x()-b.x()),
            std::abs(a.y()-b.y())
        };
    }

private:
    bool is_zooming = false;
    Qt::MouseButton zooming_button;
    QPoint zoom_start;
    QRubberBand* rubber_band;
};

} // namespace rainynite::studio::tools

namespace rainynite::studio {
REGISTER_CANVAS_TOOL(tools::ZoomArea, Canvas, 0x01);
}
