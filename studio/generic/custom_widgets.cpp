/*  custom_widgets.cpp - widgets for value editing
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

#include <core/util/class_init.h>

#include "custom_widgets.h"

namespace rainynite::studio {

unique_ptr<QWidget> new_custom_widget(std::type_index type) {
    try {
        return class_init::type_info<AbstractFactory<QWidget>, unique_ptr<QWidget>>(type);
    } catch (class_init::RuntimeTypeError const&) {
        return nullptr;
    }
}

} // namespace rainynite::studio
