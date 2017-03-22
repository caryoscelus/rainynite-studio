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
#include <thread>

#include <QDockWidget>

#include <core/context.h>

namespace Ui {
class TimeDock;
}

namespace studio {

/**
 * Dock for displaying/editing time.
 *
 * TODO:
 *  - separate time editing widget
 *  - time line
 *  - generalize ContextDock / ContextWidget
 */
class TimeDock : public QDockWidget {
    Q_OBJECT

public:
    explicit TimeDock(std::shared_ptr<core::Context> context_, QWidget* parent = 0);
    ~TimeDock();

public Q_SLOTS:
    void set_context(std::shared_ptr<core::Context> context_) {
        context = context_;
    }

protected:
    virtual void closeEvent(QCloseEvent* event) override;

protected Q_SLOTS:
    virtual void change_time(double t);

private:
    std::unique_ptr<Ui::TimeDock> ui;
    std::shared_ptr<core::Context> context;
};

}

#endif
