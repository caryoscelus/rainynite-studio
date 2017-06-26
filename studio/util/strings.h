/*
 *  strings.h - Qt <-> std string conversions
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

#ifndef __STUDIO__UTIL_STRINGS_H__5FD72FC8
#define __STUDIO__UTIL_STRINGS_H__5FD72FC8

#include <type_traits>

namespace util {

template <typename S, typename T>
T str(S&& string);

// TODO: C++17 is_same_v
template <typename S>
std::enable_if_t<std::is_same<std::decay_t<S>,std::string>::value, QString>
str(S&& string) {
    return QString::fromStdString(std::forward<S>(string));
}

template <typename S>
std::enable_if_t<std::is_same<std::decay_t<S>,QString>::value, std::string>
str(S&& string) {
    return string.toStdString();
}

}

#endif
