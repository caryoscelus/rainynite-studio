/*
 *  timearea_dock.h - time dock
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

#ifndef __STUDIO__TIMEAREA_DOCK_H__13AFC148
#define __STUDIO__TIMEAREA_DOCK_H__13AFC148

#include <memory>

#include <core/context.h>

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class TimeareaDock;
}

namespace studio {

/**
 * Time area dock
 */
class TimeareaDock : public DockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit TimeareaDock(std::shared_ptr<EditorContext> context_, QWidget* parent = nullptr);
    virtual ~TimeareaDock();

private:
    std::unique_ptr<Ui::TimeareaDock> ui;
};

REGISTER_DOCK("Timeline", TimeareaDock, Qt::BottomDockWidgetArea);

}

#endif
