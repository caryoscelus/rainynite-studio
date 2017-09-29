/*  advanced_color_dock.h - Advanced color selector dock
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

#ifndef STUDIO_DOCK_ADVANCED_COLOR_DOCK_H_93DA4B12_1912_5361_839D_FB0A6D16248D
#define STUDIO_DOCK_ADVANCED_COLOR_DOCK_H_93DA4B12_1912_5361_839D_FB0A6D16248D

#include <generic/context_listener.h>
#include <generic/dock_registry.h>
#include "close_destroy_dock.h"

namespace Ui {
class AdvancedColorDock;
}

namespace rainynite::studio {

/**
 * Advanced Color Selector Dock
 */
class AdvancedColorDock : public DockWidget, public ContextListener {
//     Q_OBJECT

public:
    explicit AdvancedColorDock(shared_ptr<EditorContext> context_, QWidget* parent_ = nullptr);
    virtual ~AdvancedColorDock();

public:
    void set_context(shared_ptr<EditorContext> context_) override;

private:
    unique_ptr<Ui::AdvancedColorDock> ui;
};

REGISTER_DOCK("Advanced Color Dock", AdvancedColorDock, Qt::LeftDockWidgetArea);

} // namespace rainynite::studio

#endif
