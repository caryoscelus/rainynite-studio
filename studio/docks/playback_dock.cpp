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

PlaybackDock::PlaybackDock(std::shared_ptr<EditorContext> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::PlaybackDock>()),
    timer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->move_start_button, SIGNAL(clicked()), this, SLOT(move_start()));
    connect(ui->move_end_button, SIGNAL(clicked()), this, SLOT(move_end()));
    connect(ui->play_button, SIGNAL(toggled(bool)), this, SLOT(toggle_playback(bool)));
    connect(ui->stop_button, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->timeline_zoom, SIGNAL(valueChanged(int)), ui->timeline, SLOT(set_zoom_level(int)));

    // TODO: move actual playing out of dock
    connect(timer, SIGNAL(timeout()), this, SLOT(next_frame()));
    set_context(get_context());
}

PlaybackDock::~PlaybackDock() {
}

void PlaybackDock::set_context(std::shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    ui->timeline->set_context(context_);
}

void PlaybackDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

void PlaybackDock::toggle_playback(bool play) {
    if (auto context = get_core_context()) {
        if (play)
            timer->start(1000/context->get_fps());
        else
            timer->stop();
    }
}

void PlaybackDock::stop() {
    ui->play_button->setChecked(false);
    move_start();
}

void PlaybackDock::next_frame() {
    if (auto context = get_core_context()) {
        auto time = context->get_time();
        if (time >= context->get_period().get_last())
            toggle_playback(false);
        else
            context->set_time(time + core::Time(0, context->get_fps(), 1));
    }
}

void PlaybackDock::move_start() {
    if (auto context = get_core_context()) {
        context->to_start();
    }
}

void PlaybackDock::move_end() {
    if (auto context = get_core_context()) {
        context->to_end();
    }
}

}
