/*  bezier_editor.cpp - bezier editor helper widget
 *  Copyright (C) 2018 caryoscelus
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

#include <core/action_stack.h>

#include <generic/node_editor.h>
#include <generic/custom_widgets.h>
#include "ui_bezier_editor.h"

namespace rainynite::studio {

class BezierEditorWidget : public QWidget, public NodeEditor {
public:
    BezierEditorWidget() :
        ui(make_unique<Ui::BezierEditorWidget>())
    {
        ui->setupUi(this);
        connect(ui->is_closed, &QCheckBox::toggled, this, &BezierEditorWidget::set_closed);
    }
    virtual ~BezierEditorWidget() {
    }

    void node_update() override {
        if (auto node = get_node_as<Geom::BezierKnots>()) {
            ui->is_closed->setCheckable(node->can_set_any_at());
            ui->is_closed->setChecked(node->value(get_core_context()).closed);
        } else {
            // error
            ui->is_closed->setCheckable(false);
            ui->is_closed->setChecked(false);
        }
    }

protected:
    void set_closed(bool closed) {
        if (auto action_stack = get_context()->action_stack()) {
            if (auto node = get_node_as<Geom::BezierKnots>()) {
                if (node->can_set_any_at()) {
                    if (auto maybe_path = get_value<Geom::BezierKnots>()) {
                        auto path = *std::move(maybe_path);
                        path.closed = closed;
                        write_value(path);
                        close_action();
                    }
                }
            }
        }
    }

private:
    unique_ptr<Ui::BezierEditorWidget> ui;
};

REGISTER_CUSTOM_WIDGET(BezierEdit, Geom::BezierKnots, BezierEditorWidget);

} // namespace rainynite::studio
