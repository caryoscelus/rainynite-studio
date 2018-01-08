/*  action.h - ui action
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

#ifndef STUDIO_GENERIC_ACTION_H_EAC1E4D4_361B_5C94_BB73_9EB2953E2613
#define STUDIO_GENERIC_ACTION_H_EAC1E4D4_361B_5C94_BB73_9EB2953E2613

#include <core/util/class_init.h>

#include "context_listener.h"

class QMenu;
class QErrorMessage;

namespace rainynite::studio {

class ContextListener;

class UiAction {
public:
    virtual void process() = 0;
};

inline map<string,UiAction*> const& get_all_actions() {
    return class_init::string_registry<UiAction>();
}

/**
 * Add all registered actions to QMenu.
 *
 * `menu` and `error_box` should never outlive `parent`
 *
 * TODO: better error-reporting
 */
void add_all_actions_to_menu(ContextListener const& parent, QMenu& menu, QErrorMessage& error_box);

#define REGISTERED_ACTION(Self) \
    private class_init::StringRegistered<Self, UiAction>

#define CONTEXT_ACTION(Self) \
    public UiAction, \
    public ContextListener, \
    REGISTERED_ACTION(Self)

#define ACTION_NAME(name_) \
public: \
    static string name() { return name_; }

} // namespace rainynite::studio

#endif
