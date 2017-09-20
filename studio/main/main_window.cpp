/*
 *  main_window.cpp - main window
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
#include <QDebug>

#include <core/document.h>
#include <core/filters/json_reader.h>
#include <core/filters/yaml_reader.h>
#include <core/filters/json_writer.h>
#include <core/filters/yaml_writer.h>

#include <version.h>
#include <util/strings.h>
#include <generic/dock_registry.h>
#include "renderer.h"
#include "about.h"
#include "main_window.h"
#include "ui_main_window.h"

using namespace fmt::literals;

namespace rainynite::studio {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ContextListener(),
    ui(make_unique<Ui::MainWindow>()),
    error_box(make_unique<QErrorMessage>())
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

    connect(ui->action_undo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->action_redo, SIGNAL(triggered()), this, SLOT(redo()));

    connect(ui->action_render, SIGNAL(triggered()), renderer.get(), SLOT(render()));
    connect(ui->action_render_frame, SIGNAL(triggered()), renderer.get(), SLOT(render_frame()));
    connect(ui->action_stop_render, SIGNAL(triggered()), renderer.get(), SLOT(stop_render()));
    connect(ui->action_redraw, SIGNAL(triggered()), renderer.get(), SLOT(redraw()));
    connect(ui->action_extra_style, SIGNAL(toggled(bool)), renderer.get(), SLOT(toggle_extra_style(bool)));

    setup_tools();

    setup_dock_menu();
    add_all_docks();

    new_document();
    renderer->render_frame();
}

MainWindow::~MainWindow() {
}

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
    unique_ptr<core::DocumentReader> json_reader = make_unique<core::filters::JsonReader>();
    unique_ptr<core::DocumentReader> yaml_reader = make_unique<core::filters::YamlReader>();
    vector<string> errors;
    for (auto&& reader : { std::move(yaml_reader), std::move(json_reader )}) {
        try {
            std::ifstream in(fname);
            document = reader->read_document(in);
            if (!document)
                throw std::runtime_error("Unknown parse failure");
            set_core_context(document->get_default_context());
            in.close();
            break;
        } catch (std::exception const& ex) {
            errors.push_back("Uncaught exception in filter:\n{}"_format(ex.what()));
        } catch (...) {
            errors.push_back("Unknown error while trying to open document via filter");
        }
    }
    if (!document) {
        auto msg = util::str(std::accumulate(errors.begin(), errors.end(), string("\n\n")));
        qDebug() << msg;
        error_box->showMessage(msg);
    }
    renderer->render_frame();
}

void MainWindow::save_as() {
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
        QString format;
        if (filter.contains("*.rnite")) {
            format = QInputDialog::getItem(this, "Choose .rnite format", "Choose save format", {"yaml", "json"});
        } else {
            error_box->showMessage("Unknown save format");
            return;
        }
        set_fname(util::str(fname_qt));
        save(format);
    }
}

void MainWindow::save(QString format) {
    if (fname.empty()) {
        save_as();
        return;
    }

    if (format.isEmpty())
        format = util::str(saved_format);
    if (format.isEmpty())
        format = "yaml";

    // TODO: proper filter modularization
    unique_ptr<core::DocumentWriter> writer;
    if (format == "yaml") {
        writer.reset(new core::filters::YamlWriter());
    } else if (format == "json") {
        writer.reset(new core::filters::JsonWriter());
    } else {
        error_box->showMessage("Unknown save format");
        return;
    }

    try {
        std::ofstream out(fname);
        writer->write_document(out, document);
        saved_format = util::str(format);
    } catch (std::exception const& ex) {
        auto msg = util::str("Uncaught exception while saving document:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while saving document";
    }
}

void MainWindow::set_fname(string const& fname_) {
    fname = fname_;
    renderer->set_fname(fname);
    update_title();
}

void MainWindow::update_title() {
    setWindowTitle(util::str(fmt::format(
        window_title_template,
        "file"_a=fname,
        "version"_a=RAINYNITE_STUDIO_VERSION,
        "codename"_a=RAINYNITE_STUDIO_CODENAME
    )));
}

void MainWindow::undo() {
    get_context()->action_stack()->undo();
}

void MainWindow::redo() {
    get_context()->action_stack()->redo();
}

void MainWindow::about() {
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::quit() {
    renderer.reset();
    QApplication::quit();
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
    // TODO
}

void MainWindow::set_context(shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    renderer->set_context(context_);
    for (auto dock : findChildren<QWidget*>()) {
        if (auto ctx_dock = dynamic_cast<ContextListener*>(dock))
            ctx_dock->set_context(context_);
        if (dock->metaObject()->indexOfSignal("activated(shared_ptr<core::AbstractValue>)") != -1)
            connect(dock, SIGNAL(activated(shared_ptr<core::AbstractValue>)), this, SLOT(activate(shared_ptr<core::AbstractValue>)));
    }
}

void MainWindow::activate(shared_ptr<core::AbstractValue> node) {
    get_context()->set_active_node(node);
}

} // namespace rainynite::studio
