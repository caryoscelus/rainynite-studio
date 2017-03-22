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
    ui(std::make_unique<Ui::TimeDock>())
{
    ui->setupUi(this);
    connect(ui->time_box, SIGNAL(valueChanged(double)), this, SLOT(change_time(double)));
    set_context(get_context());
}

TimeDock::~TimeDock() {
}

void TimeDock::set_context(std::shared_ptr<core::Context> context_) {
    ContextListener::set_context(context_);
    if (auto context = get_context()) {
        auto connection = context->changed_time.connect([this](core::Time time) {
            set_time(time);
        });
    }
}

void TimeDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

void TimeDock::change_time(double t) {
    if (auto context = get_context())
        context->set_time(core::Time(t));
}

void TimeDock::set_time(core::Time time) {
    ui->time_box->setValue(time.get_seconds());
}

}
