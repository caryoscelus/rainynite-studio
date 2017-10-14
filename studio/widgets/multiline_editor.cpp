/*  multiline_editor.cpp - edit string with multi-line text editor
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

#include <QTextEdit>

#include <util/strings.h>
#include <generic/custom_widgets.h>
#include <generic/node_editor_widget.h>

namespace rainynite::studio {

class MultilineEditor : public QTextEdit {
public:
    MultilineEditor(QWidget* parent=nullptr) :
        QTextEdit(parent)
    {}

    string value() const {
        return util::str(toPlainText());
    }

    void update_value(string const& value) {
        setPlainText(util::str(value));
    }

    virtual void write_action() = 0;

protected:
    void focusOutEvent(QFocusEvent* event) override {
        QTextEdit::focusOutEvent(event);
        write_action();
    }
};

REGISTER_CUSTOM_WIDGET(MultilineTextEdit, string, (NodeEditorWidget<MultilineEditor, string>));

} // namespace rainynite::studio
