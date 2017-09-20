/*
 *  dock_registry.h - Dock registry
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

#ifndef __STUDIO__GENERIC__DOCK_REGISTRY_H__A4BF97D0
#define __STUDIO__GENERIC__DOCK_REGISTRY_H__A4BF97D0

#include <core/std/memory.h>

#include <Qt>

#include <core/class_init.h>

class QDockWidget;

namespace rainynite::studio {

class EditorContext;

class DockFactory {
public:
    virtual unique_ptr<QDockWidget> operator()(shared_ptr<EditorContext> context) const = 0;
    virtual Qt::DockWidgetArea preferred_area() const = 0;
};

inline unique_ptr<QDockWidget> spawn_dock(std::string const& name, shared_ptr<EditorContext> context) {
    return class_init::name_info<DockFactory>(name)(context);
}

inline Qt::DockWidgetArea dock_preferred_area(std::string const& name) {
    return class_init::name_info<DockFactory>(name).preferred_area();
}

inline map<std::string, DockFactory*> const& get_all_docks() {
    return class_init::string_registry<DockFactory>();
}

} // namespace rainynite::studio

#define REGISTER_DOCK(Name, Dock, position) \
class Dock##Factory : \
    public DockFactory, \
    private class_init::StringRegistered<Dock##Factory, DockFactory> \
{ \
public: \
    static std::string name() { \
        return Name; \
    } \
public: \
    unique_ptr<QDockWidget> operator()(shared_ptr<EditorContext> context) const override { \
        return unique_ptr<QDockWidget>(static_cast<QDockWidget*>(new Dock(std::move(context)))); \
    } \
    Qt::DockWidgetArea preferred_area() const override { \
        return position; \
    } \
}

#endif
