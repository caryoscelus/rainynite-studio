/*
 *  time_dock.cpp - time dock
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

#include "time_dock.h"
#include "ui_time_dock.h"

namespace studio {

TimeDock::TimeDock(std::shared_ptr<core::Context> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::TimeDock>()),
    destroy_detector(std::make_shared<Null>())
{
    ui->setupUi(this);
    connect(ui->time_box, SIGNAL(editingFinished()), this, SLOT(change_time()));
    connect(ui->fps_box, SIGNAL(editingFinished()), this, SLOT(change_fps()));
    set_context(get_context());
}

TimeDock::~TimeDock() {
}

void TimeDock::time_changed(core::Time time) {
    set_time(time);
}

void TimeDock::fps_changed(core::Time::fps_type fps) {
    ui->fps_box->setValue(fps);
}

void TimeDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

void TimeDock::change_time() {
    if (auto context = get_context())
        context->set_time(core::Time(0, context->get_fps(), ui->time_box->value()));
}

void TimeDock::change_fps() {
    if (auto context = get_context())
        context->set_fps(ui->fps_box->value());
}

void TimeDock::set_time(core::Time time) {
    ui->time_box->setValue(time.get_frames());
}

}
