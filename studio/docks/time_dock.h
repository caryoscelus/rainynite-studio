/*
 *  time_dock.h - time dock
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

#ifndef __STUDIO__TIME_DOCK_H__476B54A4
#define __STUDIO__TIME_DOCK_H__476B54A4

#include <memory>

#include <core/context.h>

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class TimeDock;
}

namespace studio {

/**
 * Dock for displaying/editing time.
 *
 * TODO:
 *  - separate time editing widget
 */
class TimeDock : public DockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit TimeDock(std::shared_ptr<EditorContext> context_, QWidget* parent = 0);
    ~TimeDock();

protected:
    virtual void time_changed(core::Time time) override;
    virtual void fps_changed(core::Time::fps_type fps) override;

protected Q_SLOTS:
    virtual void change_time();
    virtual void change_fps();

protected:
    void set_time(core::Time time);

private:
    std::unique_ptr<Ui::TimeDock> ui;

    struct Null {};
    std::shared_ptr<Null> destroy_detector;
};

REGISTER_DOCK("Time", TimeDock, Qt::BottomDockWidgetArea);

}

#endif
