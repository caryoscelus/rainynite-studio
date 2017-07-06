/*
 *  timeline_area.h - advanced timeline
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

#ifndef __STUDIO__WIDGETS__TIMELINE_AREA_H__9A8A8D52
#define __STUDIO__WIDGETS__TIMELINE_AREA_H__9A8A8D52

#include <memory>

#include <QGraphicsView>

#include <generic/context_listener.h>

class QGraphicsScene;

namespace studio {

class TimelineArea : public QGraphicsView, public ContextListener {
    Q_OBJECT

public:
    explicit TimelineArea(QWidget* parent = nullptr);

protected:
    virtual void time_changed(core::Time) override;

private:
    std::unique_ptr<QGraphicsScene> the_scene;
//     std::vector<std::unique_ptr<TimelineEditor>> editors;
};

}

#endif
