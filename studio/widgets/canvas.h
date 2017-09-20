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

#include <core/std/memory.h>
#include <core/std/vector.h>

#include <canvas/registry.h>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsPixmapItem;
class QGraphicsRectItem;

namespace rainynite::studio {

class Canvas : public AbstractCanvas {
public:
    explicit Canvas(QWidget* parent = nullptr);
    virtual ~Canvas();

    void set_main_image(QPixmap const& pixmap);

protected:
    void set_context(shared_ptr<EditorContext> context) override;

private:
    void update_border();

private:
    unique_ptr<QGraphicsPixmapItem> image;
    unique_ptr<QGraphicsRectItem> image_border;
};

REGISTER_CANVAS(Canvas);

}

#endif
