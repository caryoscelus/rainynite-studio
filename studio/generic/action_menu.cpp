/*  action_menu.cpp - ui actions menu
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

#include <fmt/format.h>

#include <QMenu>
#include <QErrorMessage>

#include <util/strings.h>
#include "action.h"

using namespace fmt::literals;

namespace rainynite::studio {

void add_all_actions_to_menu(ContextListener const& parent, QMenu& menu, QErrorMessage& error_box) {
    for (auto const& e : get_all_actions()) {
        auto action = e.second;
        menu.addAction(
            util::str(e.first),
            [&parent, &error_box, action]() {
                if (auto context_listener = dynamic_cast<ContextListener*>(action))
                    context_listener->set_context(parent.get_context());
                try {
                    action->process();
                } catch (std::exception const& ex) {
                    error_box.showMessage(util::str("Action failed: {}"_format(ex.what())));
                }
            }
        );
    }
}

} // namespace rainynite::studio
