/*  about.h - about dialog
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

#ifndef STUDIO_MAIN_ABOUT_H_1D61A388_DD61_5E9C_AEE9_B37FC2DFB6B9
#define STUDIO_MAIN_ABOUT_H_1D61A388_DD61_5E9C_AEE9_B37FC2DFB6B9

#include <core/std/memory.h>

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class QMediaPlayer;

namespace rainynite::studio {

class AboutDialog : public QDialog {
public:
    explicit AboutDialog(QWidget* parent = nullptr);
    ~AboutDialog();

    bool eventFilter(QObject* target, QEvent* event);

    void play_video();

private:
    unique_ptr<Ui::AboutDialog> ui;
    unique_ptr<QMediaPlayer> player;
};

} // namespace rainynite::studio

#endif
