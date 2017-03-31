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

void TimelineView::set_context(std::shared_ptr<core::Context> context_) {
    ContextListener::set_context(context_);
    if (auto context = get_context()) {
        auto lambda = [this](core::Time time) {
            update();
        };
        auto slot = decltype(context->changed_time)::slot_type(lambda);
        slot.track_foreign(destroy_detector);
        context->changed_time.connect(slot);
    }
}

void TimelineView::paintEvent(QPaintEvent* paintEvent) {
    if (auto context = get_context()) {
        QPainter painter(this);
        painter.setPen(time_cursor_pen);
        int x = context->get_time().get_frames() * 4;
        painter.drawLine(x, 0, x, height());
    }
}

void TimelineView::mousePressEvent(QMouseEvent* event) {
    qDebug() << "Press";
}

void TimelineView::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "Release";
}

void TimelineView::mouseMoveEvent(QMouseEvent* event) {
    qDebug() << "Move";
}

}
