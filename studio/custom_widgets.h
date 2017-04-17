/*
 *  custom_widgets.h - widgets for value editing
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

#ifndef __STUDIO__CUSTOM_WIDGET_H__29DB1902
#define __STUDIO__CUSTOM_WIDGET_H__29DB1902

#include <typeindex>

#include <QWidget>

namespace studio {

class CustomWidgetFactory {
public:
    virtual QWidget* operator()() const = 0;
};

QWidget* new_custom_widget(std::type_index type);

}

#endif
