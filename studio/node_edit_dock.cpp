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

#include "node_model.h"
#include "node_edit_dock.h"
#include "ui_node_edit_dock.h"

using namespace fmt::literals;

namespace studio {

NodeEditDock::NodeEditDock(std::shared_ptr<core::Context> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    ui->edit->setReadOnly(true);
    connect(ui->edit, SIGNAL(editingFinished()), this, SLOT(write_node()));
    set_context(get_context());
}

void NodeEditDock::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
    if (node->is_const()) {
        try {
            auto s = core::serialize::value_to_string(node->any());
            ui->edit->setText(QString::fromStdString(s));
            ui->edit->setReadOnly(false);
        } catch (class_init::RuntimeTypeError ex) {
            auto s = "<Type Exception: {}>"_format(ex.what());
            ui->edit->setText(QString::fromStdString(s));
            ui->edit->setReadOnly(true);
        }
    } else {
        ui->edit->setText("");
        ui->edit->setReadOnly(true);
    }
    active_node = node;
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
    if (ui->edit->isReadOnly())
        return;
    qDebug() << ui->edit->text();
    auto text = ui->edit->text().toStdString();
    active_node->set_any(core::parse_primitive_type(active_node->get_type(), text));
}

} // namespace studio
