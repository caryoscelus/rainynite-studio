/*  geom.h - Qt <-> 2geom conversions
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

#ifndef STUDIO_UTIL_GEOM_H_FB758F4A_2FB3_527A_81CE_74C1E91A4DFF
#define STUDIO_UTIL_GEOM_H_FB758F4A_2FB3_527A_81CE_74C1E91A4DFF

#include <QPoint>

#include <2geom/point.h>

#include <core/std/traits.h>

namespace rainynite::util {

template <typename S, typename T>
T point(S&& p);

template <typename S>
std::enable_if_t<is_same_v<std::decay_t<S>,Geom::Point>, QPointF>
point(S&& p) {
    return { p.x(), p.y() };
}

template <typename S>
std::enable_if_t<is_same_v<std::decay_t<S>,QPointF>, Geom::Point>
point(S&& p) {
    return { p.x(), p.y() };
}

template <typename S>
std::enable_if_t<is_same_v<std::decay_t<S>,QPoint>, Geom::Point>
point(S&& p) {
    return { (Geom::Coord) p.x(), (Geom::Coord) p.y() };
}

} // namespace rainynite::util

#endif
