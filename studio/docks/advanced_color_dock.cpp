/*  advanced_color_dock.cpp - Advanced color selector dock
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

#include "advanced_color_dock.h"
#include "ui_advanced_color_dock.h"

namespace rainynite::studio {

AdvancedColorDock::AdvancedColorDock(shared_ptr<EditorContext> context_, QWidget* parent_) :
    DockWidget(parent_),
    ContextListener(context_),
    ui(make_unique<Ui::AdvancedColorDock>())
{
    ui->setupUi(this);
    set_context(get_context());
}

AdvancedColorDock::~AdvancedColorDock() {
}

void AdvancedColorDock::set_context(shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
}

} // namespace rainynite::studio
