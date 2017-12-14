/*  undo_model.cpp - undo/redo action list model
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

#include <core/action_stack.h>

#include "undo_model.h"

namespace rainynite::studio {

UndoModel::UndoModel(shared_ptr<core::ActionStack> action_stack_, QObject* parent) :
    QAbstractItemModel(parent),
    action_stack(action_stack_)
{
    auto update = [this]() {
        beginResetModel();
        endResetModel();
    };
    connect_boost(action_stack->undone_or_redone, update);
    connect_boost(action_stack->action_closed, update);
}

UndoModel::~UndoModel() {
}

QVariant UndoModel::data(QModelIndex const& index, int role) const {
    if (!index.isValid() || index.column() != 0 || index.parent().isValid())
        return {};

    switch (role) {
        case Qt::DisplayRole:
            return "action";
        default:
            return {};
    }
}

Qt::ItemFlags UndoModel::flags(QModelIndex const& /*index*/) const {
    return Qt::ItemIsEnabled;
}

QVariant UndoModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const {
    return {};
}

QModelIndex UndoModel::index(int row, int column, QModelIndex const& parent) const {
    if (parent.isValid())
        return {};
    return createIndex(row, column);
}

QModelIndex UndoModel::parent(QModelIndex const& /*index*/) const {
    return {};
}

int UndoModel::rowCount(QModelIndex const& parent) const {
    if (parent.isValid())
        return 0;
    return
        action_stack->get_undo_stack().size() +
        action_stack->get_redo_stack().size();
}

int UndoModel::columnCount(QModelIndex const& /*parent*/) const {
    return 1;
}

unique_ptr<core::AbstractAction> const& UndoModel::get_action(QModelIndex const& index) const {
    static unique_ptr<core::AbstractAction> static_null = nullptr;
    if (index.parent().isValid())
        return static_null;

    auto const& undo_stack = action_stack->get_undo_stack();
    auto undo_index = index.row();
    ptrdiff_t redo_index = undo_index - undo_stack.size();
    if (redo_index >= 0) {
        auto const& redo_stack = action_stack->get_redo_stack();
        if (redo_index >= (ptrdiff_t)redo_stack.size())
            return static_null;
        return redo_stack[redo_index];
    }
    return undo_stack[undo_index];
}

void UndoModel::reset() {
    beginResetModel();
    endResetModel();
}

} // namespace rainynite::studio
