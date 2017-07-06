/*
 *  timeline_area.cpp - advanced timeline
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

#include <QGraphicsScene>

#include "timeline_area.h"

namespace studio {

TimelineArea::TimelineArea(QWidget* parent) :
    QGraphicsView(parent),
    the_scene(std::make_unique<QGraphicsScene>())
{
}

void TimelineArea::time_changed(core::Time /*time*/) {
}

} // namespace studio
