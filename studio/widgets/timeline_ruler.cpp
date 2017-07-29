/*
 *  timeline_ruler.cpp - timeline ruler
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

using util::str;
using namespace fmt::literals;

namespace studio {

TimelineRuler::TimelineRuler(QWidget* parent) :
    QWidget(parent),
    pen {QGuiApplication::palette().text(), 1, Qt::SolidLine},
    bold_pen {QGuiApplication::palette().text(), 2, Qt::SolidLine}
{
}

QSize TimelineRuler::sizeHint() const {
    return {16, 16};
}

void TimelineRuler::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    auto start_pos = zero_pos % step;
    int sec = -(zero_pos / step);
    painter.setPen(pen);
    painter.setFont({"sans", 8});
    for (int x = start_pos; x < width(); x += step) {
        if (sec % 5 == 0)
            painter.setPen(bold_pen);
        painter.drawLine(x, 0, x, height());
        painter.setPen(pen);
        painter.drawText(x + 2, 12, str("{}"_format(std::abs(sec))));
        ++sec;
    }
}

void TimelineRuler::set_scroll(int zero_pos_) {
    zero_pos = zero_pos_;
    update();
}

void TimelineRuler::set_zoom(unsigned step_) {
    step = step_;
    update();
}

}
