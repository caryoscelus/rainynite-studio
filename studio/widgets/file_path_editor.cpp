/*  file_path_editor.cpp - edit file path with open dialogue
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

#include <QLayout>
#include <QToolButton>
#include <QFileDialog>

#include <generic/custom_widgets.h>
#include <generic/node_editor_widget.h>
#include <util/strings.h>
#include "file_path_editor.h"

namespace rainynite::studio {

FilePathEditor::FilePathEditor(QWidget* parent) :
    QWidget(parent)
{
    auto open_button = new QToolButton();
    open_button->setIcon(QIcon::fromTheme("document-open"));

    auto hlayout = new QHBoxLayout();
    hlayout->addWidget(open_button);

    setLayout(hlayout);

    connect(open_button, &QToolButton::clicked, [this]() {
        // TODO: use correct dir
        auto fname = QFileDialog::getOpenFileName(this, "Edit file path", "", "RainyNite file (*.rnite)(*.rnite);;All files(*)");
        if (fname.isEmpty())
            return;
        path = util::str(fname);
        Q_EMIT editingFinished();
    });
}

using FilePathEditWidget = NodeEditorWidget<FilePathEditor, core::fs::Path::path_t>;
REGISTER_CUSTOM_WIDGET(FilePathEdit, core::fs::Path::path_t, FilePathEditWidget);

} // namespace rainynite::studio
