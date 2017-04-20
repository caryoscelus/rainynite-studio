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
#include <thread>

#include <QMainWindow>

#include <core/context.h>
#include <core/node.h>

namespace Ui {
class MainWindow;
}

class QErrorMessage;

namespace studio {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void set_mainarea_image(std::string const& fname);

private Q_SLOTS:
    void open();
    void reload();
    void save();
    void save_as();

    void about();
    void quit();

    void tool_mouse();
    void tool_zoom();

    void render();
    void redraw();

    void set_context(std::shared_ptr<core::Context> context_);
    void activate(core::AbstractReference node);

    void add_time_dock();
    void add_playback_dock();
    void add_node_tree_dock();
    void add_node_edit_dock();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<QErrorMessage> error_box;
    std::shared_ptr<core::Document> document;
    std::shared_ptr<core::Context> context;
    std::shared_ptr<core::AbstractValue> active_node;
    std::thread render_thread;
    std::string fname;
};

}

#endif

