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
#include <QTimer>

#include <core/context.h>

#include <generic/context_listener.h>

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
    explicit PlaybackDock(std::shared_ptr<EditorContext> context_, QWidget* parent = 0);
    ~PlaybackDock();

public:
    virtual void set_context(std::shared_ptr<EditorContext> context_) override;

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private Q_SLOTS:
    void toggle_playback(bool play);
    void stop();
    void next_frame();
    void move_start();
    void move_end();

private:
    std::unique_ptr<Ui::PlaybackDock> ui;
    QTimer* timer;
};

}

#endif
