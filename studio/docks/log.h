/*  docks/log.h - global log
 *  Copyright (C) 2018 caryoscelus
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

#ifndef STUDIO_DOCKS_LOG_H_CFC4FC28_FAB2_56B1_86EA_B7766369B6DA
#define STUDIO_DOCKS_LOG_H_CFC4FC28_FAB2_56B1_86EA_B7766369B6DA

#include <fstream>

#include <core/std/memory.h>

#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class LogDock;
}

namespace rainynite::studio {

/**
 * Global log
 */
class LogDock : public DockWidget {
public:
    explicit LogDock(QWidget* parent = nullptr);
    ~LogDock();

    class RedirectStream;

private:
    unique_ptr<Ui::LogDock> ui;
    unique_ptr<RedirectStream> buff;

    observer_ptr<std::streambuf> old_cout;
    observer_ptr<std::streambuf> old_cerr;

    unique_ptr<QTimer> timer;
};

REGISTER_DOCK("Log", LogDock, Qt::BottomDockWidgetArea);

} // namespace rainynite::studio

#endif
