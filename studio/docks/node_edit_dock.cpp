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
#include <core/types.h>
#include <core/serialize/node_writer.h>

#include <util/strings.h>
#include <generic/custom_widgets.h>
#include <generic/node_editor.h>
#include "node_edit_dock.h"
#include "ui_node_edit_dock.h"

using namespace fmt::literals;

namespace rainynite::studio {

NodeEditDock::NodeEditDock(std::shared_ptr<EditorContext> context_, QWidget* parent) :
    DockWidget(parent),
    ui(std::make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    custom_widget = ui->custom_placeholder;
    ui->text_edit->setReadOnly(true);
    connect(ui->text_edit, SIGNAL(editingFinished()), this, SLOT(write_node()));
    set_context(context_);
}

NodeEditDock::~NodeEditDock() {
}

void NodeEditDock::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
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

void NodeEditDock::node_update() {
    // custom should be capable of updating itself..
    update_generic();
}

void NodeEditDock::update_value() {
    update_generic();
    update_custom();
}

void NodeEditDock::update_generic() {
    if (!active_node)
        return;
    bool writeable = active_node->is_const();
    boost::any value;
    try {
        if (writeable) {
            value = active_node->any();
        } else {
            value = active_node->get_any(get_core_context());
        }
        auto s = core::serialize::value_to_string(value);
        ui->text_edit->setText(util::str(s));
    } catch (class_init::RuntimeTypeError const& ex) {
        auto s = "<Type Exception: {}>"_format(ex.what());
        ui->text_edit->setText(util::str(s));
        writeable = false;
    } catch (std::exception const& ex) {
        auto s = "<Uncaught Exception: {}>"_format(ex.what());
        ui->text_edit->setText(util::str(s));
        writeable = false;
    }
    ui->text_edit->setReadOnly(!writeable);
}

void NodeEditDock::setup_custom_widget(std::shared_ptr<core::AbstractValue> node) {
    QWidget* widget = new_custom_widget(node->get_type());
    if (!widget) {
        widget = new QWidget();
    }
    ui->content->layout()->replaceWidget(custom_widget, widget);
    delete custom_widget;
    custom_widget = widget;
    update_custom();
}

void NodeEditDock::update_custom() {
    if (auto node_editor = dynamic_cast<NodeEditor*>(custom_widget)) {
        node_editor->set_node(active_node);
    }
    if (auto listener = dynamic_cast<ContextListener*>(custom_widget)) {
        listener->set_context(get_context());
    }
}

void NodeEditDock::write_node() {
    if (!active_node) {
        // this shouldn't really happen
        return;
    }
    if (ui->text_edit->isReadOnly())
        return;
    auto text = ui->text_edit->text().toStdString();
    try {
        active_node->set_any(core::parse_primitive_type(active_node->get_type(), text));
    } catch (std::exception const& ex) {
        qDebug() << "Exception while parsing input:" << ex.what();
        active_node_changed(active_node);
    }
}

} // namespace rainynite::studio
