/*
 *  canvas.h - main canvas widget
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

#ifndef __STUDIO__CANVAS_H__4539A958
#define __STUDIO__CANVAS_H__4539A958

#include <memory>

#include <QGraphicsView>

#include <generic/context_listener.h>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsPixmapItem;

namespace core {
class AbstractValue;
}

namespace studio {

class Canvas : public QGraphicsView, public ContextListener {
public:
    explicit Canvas(QWidget* parent = nullptr);
    virtual ~Canvas();

public:
    void set_main_image(QPixmap const& pixmap);

protected:
    virtual void time_changed(core::Time) override;
    virtual void active_node_changed(std::shared_ptr<core::AbstractValue> node) override;

private:
    void redraw_selected_node();

private:
    std::unique_ptr<QGraphicsScene> scene;
    std::unique_ptr<QGraphicsPixmapItem> image;
    std::vector<std::unique_ptr<QGraphicsItem>> knot_items;
    std::shared_ptr<core::AbstractValue> active_node;
};

}

#endif

