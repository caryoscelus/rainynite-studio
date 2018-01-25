/*  docks/node_edit.cpp - simple text editing of selecting node
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

#include <fmt/format.h>

#include <core/document.h>
#include <core/util/type_info.h>
#include <core/serialize/node_writer.h>

#include <util/strings.h>
#include <generic/custom_widgets.h>
#include <generic/node_editor.h>
#include "node_edit.h"
#include "ui_node_edit.h"

using namespace fmt::literals;

namespace rainynite::studio {

NodeEditDock::NodeEditDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ui(make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    custom_widget.reset(ui->custom_placeholder);
    ui->text_edit->setReadOnly(true);
    set_context(context_);
    connect(
        ui->enable_autoupdate,
        &QAbstractButton::toggled,
        [this](bool value) {
            ui->text_edit->set_update_enabled(value);
            if (auto node_editor = dynamic_cast<NodeEditor*>(custom_widget.get())) {
                node_editor->set_update_enabled(value);
            }
        }
    );
}

NodeEditDock::~NodeEditDock() {
}

void NodeEditDock::active_node_index_changed(core::NodeTreeIndex index) {
    set_node(index);
    active_node = index;
    if (!index)
        return;
    update_generic();
    setup_custom_widget(get_node());
}

void NodeEditDock::time_changed(core::Time time) {
    ContextListener::time_changed(time);
    if (ui->enable_autoupdate->isChecked())
        update_value();
}

void NodeEditDock::update_value() {
    update_generic();
    update_custom();
}

void NodeEditDock::setup_custom_widget(shared_ptr<core::AbstractValue> node) {
    if (!node)
        return;
    auto widget = new_custom_widget(node->get_type());
    if (!widget) {
        widget = make_unique<QWidget>();
    }
    ui->content->layout()->replaceWidget(custom_widget.get(), widget.get());
    custom_widget = std::move(widget);
    update_custom();
}

void NodeEditDock::update_generic() {
    ui->text_edit->set_node(active_node);
    ui->text_edit->set_context(get_context());
}

void NodeEditDock::update_custom() {
    if (auto listener = dynamic_cast<ContextListener*>(custom_widget.get())) {
        listener->set_context(get_context());
    }
    if (auto node_editor = dynamic_cast<NodeEditor*>(custom_widget.get())) {
        node_editor->set_node(active_node);
    }
}

} // namespace rainynite::studio
