/*
 *  timeline_view.h - simple timeline
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

#ifndef __STUDIO__TIME_DOCK_H__476B54A4
#define __STUDIO__TIME_DOCK_H__476B54A4

#include <QWidget>
#include <QPen>

#include "context_listener.h"

namespace studio {

class TimelineView : public QWidget, public ContextListener {
//     Q_OBJECT

public:
    explicit TimelineView(QWidget* parent);

public:
    virtual QSize sizeHint() const override;

public:
    virtual void set_context(std::shared_ptr<core::Context> context_) override;

protected:
    virtual void paintEvent(QPaintEvent* paintEvent) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPen time_cursor_pen;

private:
    struct Null {};
    std::shared_ptr<Null> destroy_detector;
};

}

#endif
