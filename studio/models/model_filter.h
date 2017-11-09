/*  models/model_filter.h
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

#ifndef STUDIO_MODELS_MODEL_FILTER_H_3CC6FF1C_6D72_55AB_B53C_CF69EC923561
#define STUDIO_MODELS_MODEL_FILTER_H_3CC6FF1C_6D72_55AB_B53C_CF69EC923561

#include <KRecursiveFilterProxyModel>

namespace rainynite::studio {

class RecursiveFilterProxyModel : public KRecursiveFilterProxyModel {
protected:
    bool acceptRow(int row, QModelIndex const& parent) const override {
        return KRecursiveFilterProxyModel::acceptRow(row, parent)
            || (parent.parent().isValid() && KRecursiveFilterProxyModel::acceptRow(parent.row(), parent.parent()));
    }
};

} // namespace rainynite::studio

#endif
