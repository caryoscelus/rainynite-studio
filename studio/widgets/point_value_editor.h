/*
 *  point_value_editor.h - 2d point editor widget
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

#ifndef __STUDIO__WIDGETS__POINT_VALUE_EDITOR_H__2CF69954
#define __STUDIO__WIDGETS__POINT_VALUE_EDITOR_H__2CF69954

#include <core/std/memory.h>

#include <QDoubleSpinBox>

#include <2geom/point.h>

namespace rainynite::studio {

class PointValueEditor : public QWidget {
    Q_OBJECT

public:
    PointValueEditor(QWidget* parent = nullptr);

public:
    inline Geom::Point value() const {
        return _value;
    }
    void setReadOnly(bool ro);

Q_SIGNALS:
    void editingFinished();

protected:
    void update_value(Geom::Point value_);

private Q_SLOTS:
    void write_x();
    void write_y();

private:
    QDoubleSpinBox* x_input;
    QDoubleSpinBox* y_input;
    Geom::Point _value;
};

} // namespace rainynite::studio

#endif
