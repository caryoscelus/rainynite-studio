/*
 *  about.cpp - about dialog
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

#include <fmt/format.h>

#include <QFile>

#include <version.h>
#include <util/strings.h>
#include "about.h"
#include "ui_about.h"

namespace rainynite::studio {

AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::AboutDialog>())
{
    ui->setupUi(this);

    auto about_formatted = fmt::format(
        util::str(ui->about_label->text()),
        RAINYNITE_STUDIO_VERSION,
        RAINYNITE_STUDIO_CODENAME
    );
    ui->about_label->setText(util::str(about_formatted));

    QFile license(":/text/COPYING.gpl3");
    if (license.open(QIODevice::ReadOnly)) {
        ui->license_text->setText(license.readAll());
        license.close();
    }

    QFile third_party(":/text/third-party.txt");
    if (third_party.open(QIODevice::ReadOnly)) {
        ui->third_party_text->setText(third_party.readAll());
        third_party.close();
    }

    connect(ui->close_button, SIGNAL(clicked()), this, SLOT(accept()));
}

AboutDialog::~AboutDialog() {
}

} // namespace rainynite::studio
