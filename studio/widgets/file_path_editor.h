/*  file_path_editor.h - edit file path with open dialogue
 *  Copyright (C) 2018 caryoscelus
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

#ifndef STUDIO_WIDGETS_FILE_PATH_EDITOR_H_B853F82F_991F_50DC_83AC_F2A8A9BA889E
#define STUDIO_WIDGETS_FILE_PATH_EDITOR_H_B853F82F_991F_50DC_83AC_F2A8A9BA889E

#include <QWidget>

namespace rainynite::studio {

class FilePathEditor : public QWidget {
    Q_OBJECT

public:
    FilePathEditor(QWidget* parent=nullptr);

    core::fs::Path::path_t value() const {
        return path;
    }

    void setReadOnly(bool ro_) {
        ro = ro_;
    }

Q_SIGNALS:
    void editingFinished();

protected:
    void update_value(core::fs::Path::path_t const& new_value) {
        path = new_value;
    }

private:
    core::fs::Path::path_t path;
    bool ro;
};

} // namespace rainynite::studio

#endif
