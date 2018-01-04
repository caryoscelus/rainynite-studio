/*  canvas.h - main canvas widget
 *  Copyright (C) 2017-2018 caryoscelus
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

#ifndef STUDIO_WIDGETS_CANVAS_H_20C351D1_AD17_5E84_B8D3_6EC2A325B3E4
#define STUDIO_WIDGETS_CANVAS_H_20C351D1_AD17_5E84_B8D3_6EC2A325B3E4

#include <core/std/memory.h>
#include <core/std/vector.h>

#include <canvas/registry.h>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsRectItem;
class QSlider;
class QToolButton;

namespace rainynite::studio {

class Canvas : public AbstractCanvas {
public:
    explicit Canvas(QWidget* parent = nullptr);
    virtual ~Canvas();

protected:
    void set_context(shared_ptr<EditorContext> context) override;
    void active_node_index_changed(core::NodeTree::Index index) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    void update_border();

private:
    unique_ptr<QGraphicsRectItem> image_border;

    observer_ptr<QSlider> zoom_slider;
    observer_ptr<QToolButton> zoom_reset;
};

REGISTER_CANVAS(Canvas);

} // namespace rainynite::studio

#endif
