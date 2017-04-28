/*
 *  editor_factory.cpp - value editors factory instances
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

#include <QDebug>
#include <QPushButton>

#include <core/class_init.h>
#include <core/node.h>
#include <core/color.h>

#include <generic/custom_widgets.h>
#include "color_button.h"

namespace studio {

REGISTER_CUSTOM_WIDGET(ColorEdit, core::colors::Color, ColorButton);

} // namespace studio
