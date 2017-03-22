/*
 *  playback_dock.h - playback controls dock
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

#ifndef __STUDIO__PLAYBACK_DOCK_H__AFB5B1D6
#define __STUDIO__PLAYBACK_DOCK_H__AFB5B1D6

#include <memory>

#include <QDockWidget>

#include <core/context.h>

#include "context_listener.h"

namespace Ui {
class PlaybackDock;
}

namespace studio {

/**
 * Dock with playback controls
 */
class PlaybackDock : public QDockWidget, public ContextListener {
    Q_OBJECT

public:
    explicit PlaybackDock(std::shared_ptr<core::Context> context_, QWidget* parent = 0);
    ~PlaybackDock();

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    std::unique_ptr<Ui::PlaybackDock> ui;
};

}

#endif
