/*
 *  close_destroy_dock.h - QDockWidget that destroys on closing
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

#ifndef __STUDIO__DOCK__CLOSE_DESTROY_DOCK_H__8531378A
#define __STUDIO__DOCK__CLOSE_DESTROY_DOCK_H__8531378A

#include <QDockWidget>

namespace studio {

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

} // namespace studio

#endif
