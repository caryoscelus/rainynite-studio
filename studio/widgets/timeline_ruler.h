/*
 *  timeline_ruler.h - simple timeline
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

#ifndef __STUDIO__WIDGETS__TIMELINE_RULER_H__B5968130
#define __STUDIO__WIDGETS__TIMELINE_RULER_H__B5968130

#include <QWidget>
#include <QPen>

#include <generic/context_listener.h>

namespace studio {

class TimelineRuler : public QWidget {
public:
    explicit TimelineRuler(QWidget* parent = nullptr);

public:
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

public:
    void set_scroll(int end_pos_);
    void set_zoom(unsigned step_);

private:
    QPen pen;
    QPen bold_pen;

private:
    unsigned step = 1;
    int zero_pos = 0;
};

}

#endif
