/*
 *  point_value_editor.cpp - 2d point editor widget
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

#include <QLayout>

#include "point_value_editor.h"

#define MAX_POINT_COORD 10000

namespace rainynite::studio {

PointValueEditor::PointValueEditor(QWidget* parent) :
    QWidget(parent),
    x_input(new QDoubleSpinBox()),
    y_input(new QDoubleSpinBox())
{
    x_input->setRange(-MAX_POINT_COORD, +MAX_POINT_COORD);
    y_input->setRange(-MAX_POINT_COORD, +MAX_POINT_COORD);

    auto vlayout = new QVBoxLayout();
    vlayout->addWidget(x_input);
    vlayout->addWidget(y_input);
    setLayout(vlayout);

    connect(x_input, SIGNAL(editingFinished()), this, SLOT(write_x()));
    connect(y_input, SIGNAL(editingFinished()), this, SLOT(write_y()));
}

void PointValueEditor::setReadOnly(bool ro) {
    x_input->setReadOnly(ro);
    y_input->setReadOnly(ro);
}

void PointValueEditor::update_value(Geom::Point value_) {
    _value = value_;
    x_input->setValue(value_.x());
    y_input->setValue(value_.y());
}

void PointValueEditor::write_x() {
    _value.x() = x_input->value();
    Q_EMIT editingFinished();
}

void PointValueEditor::write_y() {
    _value.y() = y_input->value();
    Q_EMIT editingFinished();
}

} // namespace rainynite::studio
