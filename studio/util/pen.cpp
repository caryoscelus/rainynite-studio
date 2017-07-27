/*
 *  pen.cpp - QPen utils
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

#include <QGuiApplication>
#include <QPalette>

#include "pen.h"

namespace studio {
namespace pens {

QPen cosmetic_dash() {
    QPen pen(QGuiApplication::palette().dark(), 1, Qt::DashLine);
    pen.setCosmetic(true);
    return pen;
}

QPen cosmetic_solid() {
    QPen pen(QGuiApplication::palette().dark(), 1, Qt::SolidLine);
    pen.setCosmetic(true);
    return pen;
}

} // namespace pens
} // namespace studio
