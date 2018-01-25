/*  custom_widgets.h - widgets for value editing
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

#ifndef STUDIO_GENERIC_CUSTOM_WIDGETS_H_37C12673_B92F_587A_B88E_37BC163EB80F
#define STUDIO_GENERIC_CUSTOM_WIDGETS_H_37C12673_B92F_587A_B88E_37BC163EB80F

#include <typeindex>

#include <QWidget>

#include <core/abstract_factory.h>
#include <core/util/class_init.h>

namespace rainynite::studio {

unique_ptr<QWidget> new_custom_widget(std::type_index type);

} // namespace rainynite::studio

#define REGISTER_CUSTOM_WIDGET(Name, Type, WidgetType) \
struct Name##Factory : \
    public AbstractFactoryImpl<QWidget, WidgetType>, \
    class_init::Registered<Name##Factory, Type, AbstractFactory<QWidget>> \
{ \
}

#endif
