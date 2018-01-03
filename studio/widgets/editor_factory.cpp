/*  editor_factory.cpp - value editors factory instances
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

#include <QPushButton>

#include <core/class_init.h>
#include <core/color/color.h>

#include <geom_helpers/knots.h>
#include <geom_helpers/rectangle.h>

#include <generic/custom_widgets.h>
#include <generic/node_editor_widget.h>
#include "color_button.h"
#include "point_value_editor.h"
#include <canvas/editors/bezier_editor.h>
#include <widgets/canvas.h>

namespace rainynite::studio {

using ColorEditWidget = NodeEditorWidget<ColorButton, core::colors::Color>;
REGISTER_CUSTOM_WIDGET(ColorEdit, core::colors::Color, ColorEditWidget);

using PointValueEditWidget = NodeEditorWidget<PointValueEditor, Geom::Point>;
REGISTER_CUSTOM_WIDGET(PointValueEdit, Geom::Point, PointValueEditWidget);

REGISTER_CANVAS_EDITOR(Canvas, BezierEditor, Geom::BezierKnots);

REGISTER_NODE_EDITOR_SHOW_CHILDREN(RectangleAB, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(CirclePR, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(RenderShape, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(PathXY, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(Translate, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(Rotate, true);
REGISTER_NODE_EDITOR_SHOW_CHILDREN(Scale, true);

REGISTER_TEMPLATE_NODE_EDITOR_SHOW_CHILDREN(Frame, "Frame", true)
REGISTER_TEMPLATE_NODE_EDITOR_SHOW_CHILDREN(List, "List", true)
REGISTER_TEMPLATE_NODE_EDITOR_SHOW_CHILDREN(ListFrame, "List/Frame", true)
REGISTER_TEMPLATE_NODE_EDITOR_SHOW_CHILDREN(Interpolate, "Interpolate", true)

} // namespace rainynite::studio
