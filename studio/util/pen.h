/*  pen.h - QPen utils
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

#ifndef STUDIO_UTIL_PEN_H_84F08A20_D07D_508B_A5B3_BBD2B2EC7E0B
#define STUDIO_UTIL_PEN_H_84F08A20_D07D_508B_A5B3_BBD2B2EC7E0B

#include <QPen>

namespace rainynite::studio::pens {

QPen cosmetic_dash();
QPen cosmetic_solid();
QPen cosmetic_thick_light_solid();

} // namespace rainynite::studio::pens

#endif
