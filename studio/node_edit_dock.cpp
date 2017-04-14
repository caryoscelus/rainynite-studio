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

#include <core/document.h>
#include <core/serialize/node_writer.h>

#include "node_model.h"
#include "node_edit_dock.h"
#include "ui_node_edit_dock.h"

namespace studio {

NodeEditDock::NodeEditDock(std::shared_ptr<core::Context> context_, QWidget* parent) :
    QDockWidget(parent),
    ContextListener(context_),
    ui(std::make_unique<Ui::NodeEditDock>())
{
    ui->setupUi(this);
    ui->edit->setReadOnly(true);
    set_context(get_context());
}

void NodeEditDock::active_node_changed(std::shared_ptr<core::AbstractValue> node) {
    if (node->is_const()) {
        auto s = core::serialize::value_to_string(node->any());
        ui->edit->setText(QString::fromStdString(s));
    }
}

NodeEditDock::~NodeEditDock() {
}

void NodeEditDock::closeEvent(QCloseEvent* event) {
    QDockWidget::closeEvent(event);
    deleteLater();
}

} // namespace studio
