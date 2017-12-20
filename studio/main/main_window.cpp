/*  main_window.cpp - main window
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

#include <fstream>

#include <fmt/format.h>

#include <QFileDialog>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QDockWidget>
#include <QActionGroup>
#include <QCloseEvent>
#include <QDebug>

#include <core/document.h>
#include <core/action_stack.h>
#include <core/filters/yaml_reader.h>
#include <core/filters/yaml_writer.h>

#include <version.h>
#include <util/strings.h>
#include <generic/dock_registry.h>
#include <generic/action.h>
#include "renderer.h"
#include "audio.h"
#include "about.h"
#include "main_window.h"
#include "ui_main_window.h"

using namespace fmt::literals;

namespace rainynite::studio {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ContextListener(),
    audio_player(make_unique<AudioPlayer>()),
    ui(make_unique<Ui::MainWindow>()),
    error_box(make_unique<QErrorMessage>()),
    tool_actions(make_unique<QActionGroup>(this))
{
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    renderer = make_shared<Renderer>(ui->canvas);
    window_title_template = util::str(windowTitle());
    update_title();

    connect(ui->action_new, SIGNAL(triggered()), this, SLOT(new_document()));
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_reload, SIGNAL(triggered()), this, SLOT(reload()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_save_as, SIGNAL(triggered()), this, SLOT(save_as()));

    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(quit()));

    connect(ui->action_undo, &QAction::triggered, [this]() {
        undo();
    });
    connect(ui->action_redo, &QAction::triggered, [this]() {
        redo();
    });

    connect(ui->action_render, SIGNAL(triggered()), renderer.get(), SLOT(render()));
    connect(ui->action_render_frame, SIGNAL(triggered()), renderer.get(), SLOT(render_frame()));
    connect(ui->action_stop_render, SIGNAL(triggered()), renderer.get(), SLOT(stop_render()));
    connect(ui->action_redraw, SIGNAL(triggered()), renderer.get(), SLOT(redraw()));
    connect(ui->action_extra_style, SIGNAL(toggled(bool)), renderer.get(), SLOT(toggle_extra_style(bool)));
    connect(ui->action_auto_redraw, SIGNAL(toggled(bool)), renderer.get(), SLOT(toggle_auto_redraw(bool)));

    connect(
        ui->action_mirror,
        &QAction::toggled,
        [this](bool value) {
            ui->canvas->mirror_horizontally(value);
        }
    );

    setup_tools();

    add_all_actions_to_menu(*this, *ui->menu_actions, *error_box);

    setup_dock_menu();
    add_all_docks();

    new_document();
    renderer->render_frame();
}

MainWindow::~MainWindow() = default;

void MainWindow::new_document() {
    document = make_shared<core::Document>();
    set_core_context(document->get_default_context());
    set_fname("");
}

void MainWindow::open() {
    auto fname_qt = QFileDialog::getOpenFileName(this, "Open", "", "RainyNite file (*.rnite)(*.rnite);;All files(*)");
    set_fname(util::str(fname_qt));
    reload();
}

void MainWindow::reload() {
    if (fname.empty())
        return;
    // TODO: proper filter modularization
    unique_ptr<core::DocumentReader> yaml_reader = make_unique<core::filters::YamlReader>();
    vector<string> errors;
    shared_ptr<core::Document> new_document;
    for (auto&& reader : { std::move(yaml_reader) }) {
        try {
            std::ifstream in(fname);
            new_document = reader->read_document(in);
            if (new_document == nullptr)
                throw std::runtime_error("Unknown parse failure");
            set_core_context(new_document->get_default_context());
            in.close();
            break;
        } catch (std::exception const& ex) {
            errors.push_back("Uncaught exception in filter:\n{}"_format(ex.what()));
        } catch (...) {
            errors.push_back("Unknown error while trying to open document via filter\n");
        }
    }
    if (new_document == nullptr) {
        auto msg = util::str(std::accumulate(errors.begin(), errors.end(), string("\n\n")));
        qDebug() << msg;
        error_box->showMessage(msg);
    }
    document = new_document;
    renderer->render_frame();
}

bool MainWindow::save_as() {
    QString filter;
    auto fname_qt = QFileDialog::getSaveFileName(
        this,
        "Save",
        util::str(fname),
        "RainyNite file (*.rnite)(*.rnite);;"
        "All files(*)",
        &filter
    );
    if (!fname_qt.isEmpty()) {
        if (!filter.contains("*.rnite")) {
            error_box->showMessage("Unknown save format");
            return false;
        }
        set_fname(util::str(fname_qt));
        return save("yaml");
    }
    return false;
}

bool MainWindow::save(QString format) {
    if (fname.empty()) {
        return save_as();
    }

    if (format.isEmpty())
        format = util::str(saved_format);
    if (format.isEmpty())
        format = "yaml";

    // TODO: proper filter modularization
    unique_ptr<core::DocumentWriter> writer;
    if (format == "yaml") {
        writer.reset(new core::filters::YamlWriter());
    } else {
        error_box->showMessage("Unknown save format");
        return false;
    }

    try {
        std::ofstream out(fname);
        writer->write_document(out, document);
        saved_format = util::str(format);
        is_saved = true;
        update_title();
        return true;
    } catch (std::exception const& ex) {
        auto msg = util::str("Uncaught exception while saving document:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while saving document";
    }
    return false;
}

void MainWindow::set_fname(string const& fname_) {
    fname = fname_;
    get_context()->set_file_name(fname);
    renderer->set_fname(fname);
    update_title();
}

void MainWindow::update_title() {
    setWindowTitle(util::str(fmt::format(
        window_title_template,
        "status"_a=is_saved ? "" : "*",
        "file"_a=fname,
        "version"_a=RAINYNITE_STUDIO_VERSION,
        "codename"_a=RAINYNITE_STUDIO_CODENAME
    )));
}

void MainWindow::about() {
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    quit();
}

void MainWindow::quit() {
    if (!is_saved && !confirm_exit())
        return;
    renderer.reset();
    QApplication::quit();
}

bool MainWindow::confirm_exit() {
    QMessageBox dialog;
    dialog.setText("Document was (perhaps) modified.");
    dialog.setInformativeText("Save changes?");
    dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    dialog.setDefaultButton(QMessageBox::Cancel);

    while (true) {
        switch (dialog.exec()) {
            case QMessageBox::Save:
                if (!save())
                    break;
                [[fallthrough]];
            case QMessageBox::Discard:
                return true;
            default:
                return false;
        }
    }
}

void MainWindow::add_all_docks() {
    for (auto const& e : get_all_docks()) {
        add_dock(e.first);
    }
}

void MainWindow::add_dock(string const& name) {
    auto dock = spawn_dock(name, get_context());
    auto position = dock_preferred_area(name);
    addDockWidget(position, dock.release());
}

void MainWindow::setup_dock_menu() {
    for (auto const& e : get_all_docks()) {
        auto name = e.first;
        ui->menu_dock->addAction(util::str(name), [this, name]() {
            add_dock(name);
        });
    }
}

void MainWindow::setup_tools() {
    connect(
        ui->canvas,
        &AbstractCanvas::tool_changed,
        [this](string const& s) {
            auto found = tool_actions_named.find(s);
            if (found != tool_actions_named.end()) {
                found->second->setChecked(true);
            }
        }
    );
    ui->canvas->load_registered_tools();
    for (auto const& e : ui->canvas->list_tools()) {
        auto const& name = e.first;
        auto tool = e.second;
        auto action = ui->tools_bar->addAction(
            QIcon::fromTheme(util::str(tool->icon())),
            util::str(name),
            [this, name] () {
                ui->canvas->use_tool(name);
            }
        );
        action->setCheckable(true);
        tool_actions->addAction(action);
        tool_actions_named.emplace(name, action);
    }
    ui->canvas->use_tool("Default");
}

void MainWindow::set_context(shared_ptr<EditorContext> context_) {
    context_->set_file_name(fname);
    ContextListener::set_context(context_);
    renderer->set_context(context_);
    audio_player->set_context(context_);
    for (auto dock : findChildren<QWidget*>()) {
        if (auto ctx_dock = dynamic_cast<ContextListener*>(dock))
            ctx_dock->set_context(context_);
        if (dock->metaObject()->indexOfSignal("activated(shared_ptr<core::AbstractValue>)") != -1)
            connect(dock, SIGNAL(activated(shared_ptr<core::AbstractValue>)), this, SLOT(activate(shared_ptr<core::AbstractValue>)));
    }
    auto set_dirty = [this]() {
        is_saved = false;
        update_title();
    };
    connect_boost(
        get_context()->action_stack()->action_closed,
        set_dirty
    );
    connect_boost(
        get_context()->action_stack()->undone_or_redone,
        set_dirty
    );
}

void MainWindow::activate(shared_ptr<core::AbstractValue> node) {
    get_context()->set_active_node(node);
}

} // namespace rainynite::studio
