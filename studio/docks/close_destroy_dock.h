/*  close_destroy_dock.h - QDockWidget that destroys on closing
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

#ifndef STUDIO_DOCKS_CLOSE_DESTROY_DOCK_H_1C635A9C_92A1_5C82_BFC0_F6D576B7A9E3
#define STUDIO_DOCKS_CLOSE_DESTROY_DOCK_H_1C635A9C_92A1_5C82_BFC0_F6D576B7A9E3

#include <QDockWidget>

namespace rainynite::studio {

class DockWidget : public QDockWidget {
public:
    DockWidget(QWidget* parent = nullptr) :
        QDockWidget(parent)
    {}
    virtual ~DockWidget()
    {}
protected:
    void closeEvent(QCloseEvent* event) override {
        QDockWidget::closeEvent(event);
        deleteLater();
    }
};

} // namespace rainynite::studio

#endif
