/*  strings.h - Qt <-> std string conversions
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

#ifndef STUDIO_UTIL_STRINGS_H_6E4FCAFB_9FD7_5EE6_9F44_8C71873BFB98
#define STUDIO_UTIL_STRINGS_H_6E4FCAFB_9FD7_5EE6_9F44_8C71873BFB98

#include <type_traits>

#include <QString>

namespace util {

template <typename S, typename T>
T str(S&& s);

// TODO: C++17 is_same_v
template <typename S>
std::enable_if_t<std::is_same<std::decay_t<S>,std::string>::value, QString>
str(S&& s) {
    return QString::fromStdString(std::forward<S>(s));
}

template <typename S>
std::enable_if_t<std::is_same<std::decay_t<S>,QString>::value, std::string>
str(S&& s) {
    return s.toStdString();
}

} // namespace util

#endif
