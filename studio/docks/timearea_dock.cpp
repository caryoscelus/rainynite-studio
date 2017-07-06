/*
 *  timearea_dock.cpp - time area / timeline dock
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

#include <widgets/timeline_area.h>

#include "timearea_dock.h"
#include "ui_timearea_dock.h"

namespace studio {

TimeareaDock::TimeareaDock(std::shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::TimeareaDock>())
{
    ui->setupUi(this);
    ui->timeline->set_context(context_);
}

TimeareaDock::~TimeareaDock() {
}

} // namespace studio
