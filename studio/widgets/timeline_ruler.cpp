/*  timeline_ruler.cpp - timeline ruler
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

#include <cmath>

#include <fmt/format.h>

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QGuiApplication>

#include <util/strings.h>
#include "timeline_ruler.h"

namespace rainynite::studio {

using util::str;
using namespace fmt::literals;

TimelineRuler::TimelineRuler(QWidget* parent) :
    QWidget(parent),
    brush(QGuiApplication::palette().brush(QPalette::Disabled, QPalette::Text)),
    pen {brush, 1, Qt::SolidLine},
    bold_pen {brush, 2, Qt::SolidLine}
{
}

QSize TimelineRuler::sizeHint() const {
    return {16, 16};
}

void TimelineRuler::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    if (step < 2)
        return;
    int sec = -(zero_pos / step);
    auto start_pos = zero_pos + sec*step;
    painter.setFont({"sans", 8});
    for (double x = start_pos; x < width(); x += step) {
        painter.setPen(pen);
        if (sec % 5 == 0) {
            painter.drawText(x + 2, 12, str("{}"_format(std::abs(sec))));
            painter.setPen(bold_pen);
        }
        painter.drawLine(x, 0, x, height());
        ++sec;
    }
}

void TimelineRuler::mousePressEvent(QMouseEvent* event) {
    if (step <= 0)
        return;
    auto s = (event->pos().x()-zero_pos)/step;
    get_core_context()->set_seconds(s);
}

void TimelineRuler::mouseMoveEvent(QMouseEvent* event) {
    mousePressEvent(event);
}

void TimelineRuler::set_scroll(int zero_pos_) {
    zero_pos = zero_pos_;
    update();
}

void TimelineRuler::set_zoom(double step_) {
    step = step_;
    update();
}

} // namespace rainynite::studio
