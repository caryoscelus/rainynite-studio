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

#include <boost/uuid/uuid_io.hpp>

#include <core/document.h>
#include <core/types.h>
#include <core/serialize/node_writer.h>

#include <generic/custom_widgets.h>
#include <generic/node_editor.h>
#include "node_edit_dock.h"
#include "ui_node_edit_dock.h"

using namespace fmt::literals;

namespace studio {

NodeEditDock::NodeEditDock(std::shared_ptr<EditorContext> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    custom_widget = ui->custom_placeholder;
    ui->text_edit->setReadOnly(true);
    connect(ui->text_edit, SIGNAL(editingFinished()), this, SLOT(write_node()));
}

void NodeEditDock::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
    active_node = node;
    if (!node) {
        ui->uid_label->setText("<no node>");
        return;
    }
    ui->uid_label->setText(QString::fromStdString(to_string(node->get_id())));
    bool writeable = node->is_const();
    boost::any value;
    try {
        if (writeable) {
            value = node->any();
        } else {
            value = node->get_any(get_core_context()->get_time());
        }
        auto s = core::serialize::value_to_string(value);
        ui->text_edit->setText(QString::fromStdString(s));
    } catch (class_init::RuntimeTypeError const& ex) {
        auto s = "<Type Exception: {}>"_format(ex.what());
        ui->text_edit->setText(QString::fromStdString(s));
        writeable = false;
    } catch (std::exception const& ex) {
        auto s = "<Uncaught Exception: {}>"_format(ex.what());
        ui->text_edit->setText(QString::fromStdString(s));
        writeable = false;
    }
    ui->text_edit->setReadOnly(!writeable);
    setup_custom_widget(node);
}

void NodeEditDock::setup_custom_widget(std::shared_ptr<core::AbstractValue> node) {
    QWidget* widget = new_custom_widget(node->get_type());
    if (!widget) {
        widget = new QWidget();
    }
    ui->content->layout()->replaceWidget(custom_widget, widget);
    delete custom_widget;
    custom_widget = widget;
    if (auto node_editor = dynamic_cast<NodeEditor*>(widget)) {
        node_editor->set_node(node);
    }
    if (auto listener = dynamic_cast<ContextListener*>(widget)) {
        listener->set_context(get_context());
    }
}

NodeEditDock::~NodeEditDock() {
}

void NodeEditDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

void NodeEditDock::write_node() {
    if (!active_node) {
        // this shouldn't really happen
        return;
    }
    if (ui->text_edit->isReadOnly())
        return;
    qDebug() << ui->text_edit->text();
    auto text = ui->text_edit->text().toStdString();
    active_node->set_any(core::parse_primitive_type(active_node->get_type(), text));
}

} // namespace studio
