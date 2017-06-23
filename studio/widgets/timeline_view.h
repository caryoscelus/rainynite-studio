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

#ifndef __STUDIO__WIDGETS__TIMELINE_VIEW_H__D9D5E306
#define __STUDIO__WIDGETS__TIMELINE_VIEW_H__D9D5E306

#include <QWidget>
#include <QPen>

#include <generic/context_listener.h>

namespace studio {

class TimelineView : public QWidget, public ContextListener {
    Q_OBJECT

public:
    explicit TimelineView(QWidget* parent = nullptr);

public:
    virtual QSize sizeHint() const override;

public Q_SLOTS:
    void set_zoom_level(int level);

protected:
    virtual void time_changed(core::Time) override;

protected:
    virtual void paintEvent(QPaintEvent* paintEvent) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
    void start_moving(double x);
    void stop_moving(double x);
    void move(double x);
    bool is_moving = false;

private:
    double frames_to_x(double frames);
    double x_to_frames(double x);
    inline double zoom_factor() {
        return std::pow(2, zoom_level);
    }

private:
    QPen time_cursor_pen;

private:
    int zoom_level;
};

}

#endif
