/*  main_window.h - main window
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

#ifndef STUDIO_MAIN_MAIN_WINDOW_H_A13C4B3D_EED9_5A8F_B608_29AB3D72761B
#define STUDIO_MAIN_MAIN_WINDOW_H_A13C4B3D_EED9_5A8F_B608_29AB3D72761B

#include <memory>

#include <QMainWindow>

#include <core/std/map.h>
#include <core/node/abstract_value.h>

#include <generic/context_listener.h>
#include "renderer.h"

namespace Ui {
class MainWindow;
}

class QErrorMessage;
class QActionGroup;

namespace rainynite::core {
class DocumentLoader;
}

namespace rainynite::studio {

class AudioPlayer;

class MainWindow : public QMainWindow, public ContextListener {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_context(shared_ptr<EditorContext> context_) override;

protected:
    void closeEvent(QCloseEvent* event) override;

private Q_SLOTS:
    void new_document();
    void open();
    void reload();
    bool save(QString format="");
    bool save_as();

    void update_title();
    void about();
    void quit();

    void add_all_docks();
    void add_dock(string const& name);

private:
    bool confirm_exit();

    void setup_dock_menu();
    void setup_tools();

    void set_fname(string const& fname_);

private:
    shared_ptr<Renderer> renderer;
    unique_ptr<AudioPlayer> audio_player;
    unique_ptr<Ui::MainWindow> ui;
    unique_ptr<QErrorMessage> error_box;
    shared_ptr<core::AbstractDocument> document;
    shared_ptr<core::AbstractValue> active_node;
    unique_ptr<QActionGroup> tool_actions;
    map<string, observer_ptr<QAction>> tool_actions_named;

    observer_ptr<core::DocumentLoader> document_loader;

    string fname;
    string saved_format;
    string window_title_template;

    bool is_saved = false;
};

} // namespace rainynite::studio

#endif
