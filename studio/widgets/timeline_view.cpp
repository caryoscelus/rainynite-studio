/*
 *  timeline_view.cpp - simple timeline
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

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

#include "timeline_view.h"

namespace studio {

TimelineView::TimelineView(QWidget* parent) :
    QWidget(parent),
    time_cursor_pen {{{0xff, 0xff, 0xff}}, 3.0},
    destroy_detector(std::make_shared<Null>())
{
}

QSize TimelineView::sizeHint() const {
    return {20, 20};
}

void TimelineView::set_zoom_level(int level) {
    zoom_level = level;
    update();
}

void TimelineView::time_changed(core::Time) {
    update();
}

void TimelineView::paintEvent(QPaintEvent* /*event*/) {
    if (auto context = get_core_context()) {
        QPainter painter(this);
        painter.setPen(time_cursor_pen);
        int x = frames_to_x(context->get_time().get_frames());
        painter.drawLine(x, 0, x, height());
    }
}

void TimelineView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        start_moving(event->x());
}

void TimelineView::mouseReleaseEvent(QMouseEvent* event) {
    if (is_moving && event->button() == Qt::LeftButton)
        stop_moving(event->x());
}

void TimelineView::mouseMoveEvent(QMouseEvent* event) {
    if (is_moving)
        move(event->x());
}

void TimelineView::start_moving(double x) {
    is_moving = true;
    move(x);
}

void TimelineView::stop_moving(double x) {
    is_moving = false;
    move(x);
}

void TimelineView::move(double x) {
    if (auto context = get_core_context()) {
        int frames = x_to_frames(x);
        context->set_frames(frames);
    }
}

double TimelineView::frames_to_x(double frames) {
    return frames * zoom_factor();
}
double TimelineView::x_to_frames(double x) {
    return x / zoom_factor();
}

}
