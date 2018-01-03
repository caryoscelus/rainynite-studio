/*  boolean_editor.cpp - edit booleans
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

#include <QCheckBox>

#include <generic/custom_widgets.h>
#include <generic/node_editor_widget.h>

namespace rainynite::studio {

class BooleanEditor : public QCheckBox {
public:
    BooleanEditor(QWidget* parent=nullptr) :
        QCheckBox(parent)
    {
        connect(this, &QAbstractButton::toggled, this, &BooleanEditor::write_action);
    }

    bool value() const {
        return isChecked();
    }

    void update_value(bool value) {
        setChecked(value);
    }

    void setReadOnly(bool ro) {
        setEnabled(!ro);
    }

    virtual void write_action() = 0;
};

using BooleanEditWidget = NodeEditorWidget<BooleanEditor, bool>;
REGISTER_CUSTOM_WIDGET(BooleanEdit, bool, BooleanEditWidget);

} // namespace rainynite::studio
