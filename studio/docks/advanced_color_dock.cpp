/*  advanced_color_dock.cpp - Advanced color selector dock
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

#include <core/color.h>
#include <core/node/abstract_value.h>

#include "advanced_color_dock.h"
#include "ui_advanced_color_dock.h"

namespace rainynite::studio {

AdvancedColorDock::AdvancedColorDock(shared_ptr<EditorContext> context_, QWidget* parent_) :
    DockWidget(parent_),
    ContextListener(context_),
    ui(make_unique<Ui::AdvancedColorDock>())
{
    ui->setupUi(this);
    set_context(get_context());
    connect(ui->color_widget, &color_widgets::AdvancedColorSelector::colorChanged, this, &AdvancedColorDock::write_color);
}

AdvancedColorDock::~AdvancedColorDock() {
}

void AdvancedColorDock::active_node_changed(shared_ptr<core::AbstractValue> node) {
    using core::BaseValue;
    using core::colors::Color;
    if (auto color_node = dynamic_cast<BaseValue<Color>*>(node.get())) {
        auto color = color_node->value(get_core_context());
        ui->color_widget->setColor({
            color.r,
            color.g,
            color.b,
            color.a
        });
    }
}

void AdvancedColorDock::write_color(QColor c) {
    using core::BaseValue;
    using core::colors::Color;
    if (auto node = dynamic_cast<BaseValue<Color>*>(get_context()->get_active_node().get())) {
        // TODO: use generic color conversion
        node->set({
            (unsigned char)c.red(),
            (unsigned char)c.green(),
            (unsigned char)c.blue(),
            // temporary fix for #41
            node->get(get_core_context()).a
        });
    }
}

} // namespace rainynite::studio
