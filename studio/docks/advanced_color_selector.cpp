/*  docks/advanced_color_selector.cpp - Advanced color selector dock
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

#include <core/color/color.h>
#include <core/node/abstract_value.h>
#include <core/node/cast.h>
#include <core/action_stack.h>
#include <core/actions/change_value.h>

#include "advanced_color_selector.h"
#include "ui_advanced_color_selector.h"

namespace rainynite::studio {

AdvancedColorDock::AdvancedColorDock(shared_ptr<EditorContext> context_, QWidget* parent_) :
    DockWidget(parent_),
    ContextListener(context_),
    ui(make_unique<Ui::AdvancedColorDock>())
{
    using namespace color_widgets;
    ui->setupUi(this);
    ui->color_widget->setEnabledWidgets(AdvancedColorSelector::Main | AdvancedColorSelector::History);
    set_context(get_context());
    connect(ui->color_widget, &AdvancedColorSelector::colorChanged, this, &AdvancedColorDock::write_color);
}

AdvancedColorDock::~AdvancedColorDock() {
}

void AdvancedColorDock::active_node_changed(shared_ptr<core::AbstractValue> node) {
    using core::base_value_cast;
    using core::colors::Color;
    if (auto color_node = base_value_cast<Color>(std::move(node))) {
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
    using core::base_value_cast;
    using core::actions::ChangeValue;
    using core::colors::Color;
    if (auto node = base_value_cast<Color>(get_context()->get_active_node())) {
        auto color = Color {
            (unsigned char)c.red(),
            (unsigned char)c.green(),
            (unsigned char)c.blue(),
            (unsigned char)c.alpha()
        };
        if (node->can_set_any(color)) {
            // TODO: use generic color conversion
            get_context()->action_stack()->emplace<ChangeValue>(
                node,
                color
            );
        }
    }
}

} // namespace rainynite::studio
