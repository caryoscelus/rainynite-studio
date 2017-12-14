/*  docks/undo_history.cpp - undo/redo history dock
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

#include <models/undo_model.h>

#include "undo_history.h"
#include "ui_undo_history.h"

namespace rainynite::studio {

UndoHistoryDock::UndoHistoryDock(shared_ptr<EditorContext> context, QWidget* parent) :
    DockWidget(parent),
    ContextListener(context),
    ui(make_unique<Ui::UndoHistoryDock>())
{
    ui->setupUi(this);
    connect(ui->undo, &QAbstractButton::clicked, [this]() {
        undo();
    });
    connect(ui->redo, &QAbstractButton::clicked, [this]() {
        redo();
    });
    connect(ui->clear, &QAbstractButton::clicked, [this]() {
        clear_undo_history();
        model->reset();
    });
}

UndoHistoryDock::~UndoHistoryDock() {
}

void UndoHistoryDock::set_context(shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    model = make_unique<UndoModel>(context->action_stack());
    ui->action_list->setModel(model.get());
}

} // namespace rainynite::studio
