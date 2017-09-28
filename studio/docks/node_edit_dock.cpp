/*
 *  node_edit_dock.cpp - simple text editing of selecting node
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

#include <QDebug>

#include <fmt/format.h>

#include <core/document.h>
#include <core/type_info.h>
#include <core/serialize/node_writer.h>

#include <util/strings.h>
#include <generic/custom_widgets.h>
#include <generic/node_editor.h>
#include "node_edit_dock.h"
#include "ui_node_edit_dock.h"

using namespace fmt::literals;

namespace rainynite::studio {

NodeEditDock::NodeEditDock(shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ui(make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    custom_widget = ui->custom_placeholder;
    ui->text_edit->setReadOnly(true);
    set_context(context_);
}

NodeEditDock::~NodeEditDock() {
}

void NodeEditDock::active_node_changed(shared_ptr<core::AbstractValue> node) {
    set_node(node);
    active_node = node;
    if (!node)
        return;
    update_generic();
    setup_custom_widget(node);
}

void NodeEditDock::time_changed(core::Time time) {
    ContextListener::time_changed(time);
    update_value();
}

void NodeEditDock::update_value() {
    update_generic();
    update_custom();
}

void NodeEditDock::setup_custom_widget(shared_ptr<core::AbstractValue> node) {
    QWidget* widget = new_custom_widget(node->get_type());
    if (!widget) {
        widget = new QWidget();
    }
    ui->content->layout()->replaceWidget(custom_widget, widget);
    delete custom_widget;
    custom_widget = widget;
    update_custom();
}

void NodeEditDock::update_generic() {
    ui->text_edit->set_node(active_node);
    ui->text_edit->set_context(get_context());
}

void NodeEditDock::update_custom() {
    if (auto node_editor = dynamic_cast<NodeEditor*>(custom_widget)) {
        node_editor->set_node(active_node);
    }
    if (auto listener = dynamic_cast<ContextListener*>(custom_widget)) {
        listener->set_context(get_context());
    }
}

} // namespace rainynite::studio
