/*  docks/playback.cpp - playback controls dock
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

#include "playback.h"
#include "ui_playback.h"

namespace rainynite::studio {

PlaybackDock::PlaybackDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context_),
    ui(make_unique<Ui::PlaybackDock>()),
    timer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->move_start_button, SIGNAL(clicked()), this, SLOT(move_start()));
    connect(ui->move_end_button, SIGNAL(clicked()), this, SLOT(move_end()));
    connect(ui->play_button, SIGNAL(toggled(bool)), this, SLOT(toggle_playback(bool)));
    connect(ui->stop_button, SIGNAL(clicked()), this, SLOT(stop()));

    connect(ui->time_box, SIGNAL(editingFinished()), this, SLOT(change_time()));
    connect(ui->fps_box, SIGNAL(editingFinished()), this, SLOT(change_fps()));

    // TODO: move actual playing out of dock
    connect(timer, SIGNAL(timeout()), this, SLOT(next_frame()));
    set_context(get_context());
}

PlaybackDock::~PlaybackDock() {
}

void PlaybackDock::time_changed(core::Time time) {
    ui->time_box->setValue(time.get_frames());
}

void PlaybackDock::fps_changed(core::Time::fps_type fps) {
    ui->fps_box->setValue(fps);
}

void PlaybackDock::change_time() {
    if (auto context = get_core_context())
        context->set_time(core::Time(0, context->get_fps(), ui->time_box->value()));
}

void PlaybackDock::change_fps() {
    if (auto context = get_core_context())
        context->set_fps(ui->fps_box->value());
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
        auto t = time;
        ++t;
        if (t == context->get_period().get_last())
            ui->play_button->setChecked(false);
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
