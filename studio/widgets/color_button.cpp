/*  color_button.cpp - choose color button
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

#include <core/color/color.h>

#include <util/strings.h>
#include "color_button.h"

using namespace fmt::literals;

namespace rainynite::studio {

ColorButton::ColorButton(QWidget* parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(choose_color()));
}

void ColorButton::setReadOnly(bool ro) {
    setEnabled(!ro);
}

void ColorButton::choose_color() {
    auto qcolor = QColor(color.r, color.g, color.b, color.a);
    auto selected = QColorDialog::getColor(qcolor, nullptr, "", QColorDialog::ShowAlphaChannel);
    update_value({
        (uint8_t) selected.red(),
        (uint8_t) selected.green(),
        (uint8_t) selected.blue(),
        (uint8_t) selected.alpha()
    });
    update_button_color();
    Q_EMIT editingFinished();
}

void ColorButton::update_button_color() {
    auto c = core::colors::to_hex24(color);
    // TODO: keep colour when focused (perhaps use icon instead of style?)
    setStyleSheet(util::str("background-color: {}"_format(c)));
}

void ColorButton::update_value(core::colors::Color color_) {
    color = color_;
    update_button_color();
}

} // namespace rainynite::studio
