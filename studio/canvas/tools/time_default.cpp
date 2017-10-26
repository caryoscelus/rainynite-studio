/*  time_default.cpp - default time area tool
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

#include <canvas/tool.h>
#include <widgets/timeline_area.h>

namespace rainynite::studio::tools {

class TimeDefault : public CanvasTool {
public:
    string icon() const override {
        return "cursor-arrow";
    }

    string name() const override {
        return global_name();
    }

    static string global_name() {
        return "Default";
    }

    bool canvas_event(QEvent* event) override {
        if (timeline_area() == nullptr)
            return false;
        if (auto wheel_event = dynamic_cast<QWheelEvent*>(event)) {
            auto scale_factor = std::pow(2, wheel_event->angleDelta().y() / 256.0);
            // TODO: proper zoom
            timeline_area()->scale(scale_factor, 1.0);
            return true;
        }
        return false;
    }

private:
    observer_ptr<TimelineArea> timeline_area() const {
        return make_observer(dynamic_cast<TimelineArea*>(get_canvas()));
    }
};

} // namespace rainynite::studio::tools

namespace rainynite::studio {
REGISTER_CANVAS_TOOL(tools::TimeDefault, TimelineArea);
}
