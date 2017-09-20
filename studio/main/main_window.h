/*
 *  main_window.h - main window
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

#ifndef __STUDIO__MAIN_WINDOW_H__D1BD80BA
#define __STUDIO__MAIN_WINDOW_H__D1BD80BA

#include <memory>

#include <QMainWindow>

#include <core/node/abstract_value.h>

#include <generic/context_listener.h>
#include "renderer.h"

namespace Ui {
class MainWindow;
}

class QErrorMessage;

namespace rainynite::studio {

class MainWindow : public QMainWindow, public ContextListener {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void set_context(shared_ptr<EditorContext> context_) override;

private Q_SLOTS:
    void new_document();
    void open();
    void reload();
    void save();
    void save_as();

    void update_title();
    void about();
    void quit();

    void undo();
    void redo();

    void activate(shared_ptr<core::AbstractValue> node);

    void add_all_docks();
    void add_dock(string const& name);

private:
    void setup_dock_menu();
    void setup_tools();

    void set_fname(string const& fname_);

private:
    shared_ptr<Renderer> renderer;
    unique_ptr<Ui::MainWindow> ui;
    unique_ptr<QErrorMessage> error_box;
    shared_ptr<core::Document> document;
    shared_ptr<core::AbstractValue> active_node;

    string fname;
    string window_title_template;
};

} // namespace rainynite::studio

#endif
