/*
 *  playback_dock.cpp - playback controls dock
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

#include "playback_dock.h"
#include "ui_playback_dock.h"

namespace studio {

PlaybackDock::PlaybackDock(std::shared_ptr<core::Context> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::PlaybackDock>())
{
    ui->setupUi(this);
}

PlaybackDock::~PlaybackDock() {
}

void PlaybackDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

}
