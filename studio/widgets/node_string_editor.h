/*  node_string_editor.h - simple text-based node editor
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

#ifndef STUDIO_WIDGETS_NODE_STRING_EDITOR_H_F0A3331B_DE7D_5F83_883F_370E122FDE44
#define STUDIO_WIDGETS_NODE_STRING_EDITOR_H_F0A3331B_DE7D_5F83_883F_370E122FDE44

#include <fmt/format.h>

#include <QLineEdit>

#include <util/strings.h>
#include <generic/node_editor_widget.h>

namespace rainynite::studio {

class QLineEditWrapper : public QLineEdit {
public:
    QLineEditWrapper(QWidget* parent=nullptr) :
        QLineEdit(parent)
    {}

    string value() const {
        return util::str(text());
    }

    void update_value(any const& value) {
        using namespace fmt::literals;
        string s;
        try {
            s = core::serialize::value_to_string(value);
        } catch (class_init::RuntimeTypeError const& ex) {
            s = "<Type Exception: {}>"_format(ex.what());
        } catch (std::exception const& ex) {
            s = "<Uncaught Exception: {}>"_format(ex.what());
        }
        setText(util::str(s));
    }
};

using NodeStringEditor = UntypedNodeEditorWidget<QLineEditWrapper>;

} // namespace rainynite::studio

#endif
