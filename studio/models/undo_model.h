/*  undo_model.h - undo/redo action list model
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

#ifndef STUDIO_MODELS_UNDO_MODEL_H_51ADE8C2_A0E9_5F83_A862_2E9B1691C0AA
#define STUDIO_MODELS_UNDO_MODEL_H_51ADE8C2_A0E9_5F83_A862_2E9B1691C0AA

#include <QAbstractItemModel>

#include <generic/context_listener.h>

namespace rainynite::core {
class AbstractAction;
class ActionStack;
} // namespace rainynite::core

namespace rainynite::studio {

class UndoModel : public QAbstractItemModel, public ContextListener {
public:
    explicit UndoModel(shared_ptr<core::ActionStack> action_stack_, QObject* parent=nullptr);
    virtual ~UndoModel();

    QVariant data(QModelIndex const& index, int role) const override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, QModelIndex const& parent={}) const override;
    QModelIndex parent(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent={}) const override;
    int columnCount(QModelIndex const& parent={}) const override;

    void reset();

protected:
    unique_ptr<core::AbstractAction> const& get_action(QModelIndex const& index) const;

private:
    shared_ptr<core::ActionStack> action_stack;
};

} // namespace rainynite::studio

#endif
