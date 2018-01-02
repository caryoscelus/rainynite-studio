/*  geom.h - Qt <-> 2geom conversions
 *  Copyright (C) 2017-2018 caryoscelus
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
#include <QMatrix>

#include <2geom/point.h>

#include <core/std/traits.h>

namespace rainynite::util {


/**
 * Convert 2D point between lib2geom and Qt (QPoint/QPointF).
 *
 * This template function can be called whenever you need the conversion and
 * direction will be detected automatically throw template specializations.
 */
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


/**
 * Convert affine transform matrices between lib2geom (Affine) and Qt (QMatrix).
 *
 * Template specializations will automatically choose appropriate instance.
 *
 * NOTE: since QTransform is full 3x3 matrix, it cannot be converted into
 * Geom::Affine without possible information loss, so you have to do that
 * explicitly.
 */
template <typename S, typename T>
T matrix(S&& m);

template <typename S>
std::enable_if_t<is_same_v<std::decay_t<S>,Geom::Affine>, QMatrix>
matrix(S&& m) {
    return {m[0], m[1], m[2], m[3], m[4], m[5]};
}

template <typename S>
std::enable_if_t<is_same_v<std::decay_t<S>,QMatrix>, Geom::Affine>
matrix(S&& m) {
    return {m.m11(), m.m12(), m.m21(), m.m22(), m.dx(), m.dy()};
}


} // namespace rainynite::util

#endif
