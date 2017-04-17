/*
 *  color_button.cpp - choose color button
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

#include <QColorDialog>
#include <QDebug>

#include <fmt/format.h>

#include <core/node.h>
#include <core/color.h>

#include "color_button.h"

using namespace fmt::literals;

namespace studio {

ColorButton::ColorButton(QWidget* parent) :
    QPushButton(parent)
{
}

void ColorButton::set_node(std::shared_ptr<core::AbstractValue> node) {
    NodeEditor::set_node(node);
    color_node = dynamic_cast<core::Value<core::colors::Color>*>(node.get());
    if (color_node) {
        update_color();
        connect(this, SIGNAL(clicked(bool)), this, SLOT(choose_color()));
    }
}

void ColorButton::choose_color() {
    auto c = color_node->mod();
    auto qcolor = QColor(c.r, c.g, c.b, c.a);
    auto selected = QColorDialog::getColor(qcolor, nullptr, "", QColorDialog::ShowAlphaChannel);
    qDebug() << selected;
    color_node->set(core::colors::Color(selected.red(), selected.green(), selected.blue(), selected.alpha()));
    update_color();
}

void ColorButton::update_color() {
    auto c = core::colors::to_hex24(color_node->mod());
    // TODO: keep colour when focused (perhaps use icon instead of style?)
    auto style = QString::fromStdString("background-color: {}"_format(c));
    setStyleSheet(style);
}

} // namespace studio
