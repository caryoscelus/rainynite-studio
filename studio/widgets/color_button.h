/*
 *  color_button.h - choose color button
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

#ifndef __STUDIO__COLOR_BUTTON_H__2CC74D64
#define __STUDIO__COLOR_BUTTON_H__2CC74D64

#include <QDebug>
#include <QPushButton>

#include <core/color.h>

#include <generic/node_editor.h>

namespace core {
template <class T>
class Value;
}

namespace studio {

class ColorButton : public QPushButton, public NodeEditor {
    Q_OBJECT
public:
    ColorButton(QWidget* parent = nullptr);
public:
    virtual void set_node(std::shared_ptr<core::AbstractValue> node_) override;
private Q_SLOTS:
    void choose_color();
private:
    void update_color();
private:
    core::Value<core::colors::Color>* color_node;
};

} // namespace studio

#endif
