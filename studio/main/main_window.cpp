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
#include <QErrorMessage>
#include <QMessageBox>
#include <QDockWidget>
#include <QDebug>

#include <core/document.h>
#include <core/filters/json_reader.h>
#include <core/filters/json_writer.h>
#include <core/renderers/svg_renderer.h>

#include <version.h>
#include <util/strings.h>
#include <generic/dock_registry.h>
#include "about.h"
#include "main_window.h"
#include "ui_main_window.h"

using namespace fmt::literals;

namespace studio {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ContextListener(),
    ui(std::make_unique<Ui::MainWindow>()),
    error_box(std::make_unique<QErrorMessage>())
{
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
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

    connect(ui->action_tool_mouse, SIGNAL(triggered()), this, SLOT(tool_mouse()));
    connect(ui->action_tool_zoom, SIGNAL(triggered()), this, SLOT(tool_zoom()));

    connect(ui->action_render, SIGNAL(triggered()), this, SLOT(render()));
    connect(ui->action_render_frame, SIGNAL(triggered()), this, SLOT(render_frame()));
    connect(this, SIGNAL(redraw_signal()), this, SLOT(redraw()));
    connect(ui->action_redraw, SIGNAL(triggered()), this, SLOT(redraw()));
    connect(ui->action_extra_style, SIGNAL(toggled(bool)), this, SLOT(toggle_extra_style(bool)));

    setup_dock_menu();
    add_all_docks();

    new_document();
    setup_renderer();
    render_frame();
}

MainWindow::~MainWindow() {
}

void MainWindow::new_document() {
    document = std::make_shared<core::Document>();
    set_core_context(document->get_default_context());
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
    try {
        auto reader = core::filters::JsonReader();
        std::ifstream in(fname);
        document = reader.read_document(in);
        if (!document)
            throw std::runtime_error("Unknown parse failure");
        set_core_context(document->get_default_context());
        in.close();
    } catch (std::exception const& ex) {
        auto msg = util::str("Uncaught exception in JSON filter:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while trying to open document via JSON filter";
    }
    render_frame();
}

void MainWindow::save_as() {
    auto fname_qt = QFileDialog::getSaveFileName(
        this,
        "Save",
        util::str(fname),
        "RainyNite file (*.rnite)(*.rnite);;All files(*)"
    );
    if (!fname_qt.isEmpty()) {
        set_fname(util::str(fname_qt));
        save();
    }
}

void MainWindow::save() {
    if (fname.empty()) {
        save_as();
        return;
    }
    try {
        auto writer = core::filters::JsonWriter();
        std::ofstream out(fname);
        writer.write_document(out, document);
    } catch (std::exception const& ex) {
        auto msg = util::str("Uncaught exception while saving document:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while saving document";
    }
}

void MainWindow::set_fname(std::string const& fname_) {
    fname = fname_;
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

void MainWindow::setup_renderer() {
    auto renderer = std::make_shared<core::renderers::SvgRenderer>();
    render_thread = std::thread([this, renderer]() {
        while (!renderer_quit) {
            if (renderer_queue.size() > 0) {
                renderer_mutex.lock();
                auto ctx = std::move(renderer_queue.front());
                renderer_queue.pop();
                renderer_mutex.unlock();

                try {
                    renderer->render(ctx);
                } catch (std::exception const& ex) {
                    auto msg = util::str("Uncaught exception while rendering:\n{}"_format(ex.what()));
                    qDebug() << msg;
                }
                Q_EMIT redraw_signal();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(128));
            }
        }
    });
}

void MainWindow::render_period(core::TimePeriod const& period) {
    if (get_core_context()->get_document()) {
        auto rsettings = core::renderers::SvgRendererSettings();
        rsettings.render_pngs = true;
        rsettings.keep_alive = true;
        rsettings.extra_style = extra_style;
        get_core_context()->mod_render_settings() = rsettings;
        auto ctx = *get_core_context();
        ctx.set_period(period);

        std::lock_guard<std::mutex> lock(renderer_mutex);
        renderer_queue.push(ctx);
    }
}

void MainWindow::render() {
    render_period(get_core_context()->get_period());
}

void MainWindow::render_frame() {
    auto time = get_core_context()->get_time();
    auto time_end = time;
    ++time_end;
    render_period({time, time_end});
}

void MainWindow::redraw() {
    set_mainarea_image("renders/{:.3f}.png"_format(get_core_context()->get_time().get_seconds()));
}

void MainWindow::toggle_extra_style(bool checked) {
    extra_style = checked;
}

void MainWindow::undo() {
    get_context()->action_stack()->undo();
}

void MainWindow::redo() {
    get_context()->action_stack()->redo();
}

void MainWindow::tool_mouse() {
    qDebug() << "mouse selected";
}

void MainWindow::tool_zoom() {
    qDebug() << "zoom selected";
}

void MainWindow::about() {
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::quit() {
    if (render_thread.joinable()) {
        renderer_quit = true;
        render_thread.join();
    }
    QApplication::quit();
}

void MainWindow::set_mainarea_image(std::string const& fname) {
    QPixmap pixmap;
    pixmap.load(util::str(fname));
    ui->canvas->set_main_image(pixmap);
}

void MainWindow::add_all_docks() {
    for (auto const& e : get_all_docks()) {
        add_dock(e.first);
    }
}

void MainWindow::add_dock(std::string const& name) {
    auto dock = spawn_dock(name, get_context());
    auto position = dock_preferred_area(name);
    addDockWidget(position, dock.get());
    docks.push_back(std::move(dock));
}

void MainWindow::setup_dock_menu() {
    for (auto const& e : get_all_docks()) {
        auto name = e.first;
        ui->menu_dock->addAction(util::str(name), [this, name]() {
            add_dock(name);
        });
    }
}

void MainWindow::set_context(std::shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    get_context()->changed_time().connect([this](core::Time){
        redraw();
    });
    for (auto dock : findChildren<QWidget*>()) {
        if (auto ctx_dock = dynamic_cast<ContextListener*>(dock))
            ctx_dock->set_context(context_);
        if (dock->metaObject()->indexOfSignal("activated(std::shared_ptr<core::AbstractValue>)") != -1)
            connect(dock, SIGNAL(activated(std::shared_ptr<core::AbstractValue>)), this, SLOT(activate(std::shared_ptr<core::AbstractValue>)));
    }
}

void MainWindow::activate(std::shared_ptr<core::AbstractValue> node) {
    get_context()->set_active_node(node);
}

}
