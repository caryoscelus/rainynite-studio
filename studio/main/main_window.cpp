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
#include <QDebug>

#include <core/document.h>
#include <core/filters/svg_path_reader.h>
#include <core/filters/json_reader.h>
#include <core/filters/json_writer.h>
#include <core/renderers/svg_renderer.h>

#include <version.h>
#include <docks/time_dock.h>
#include <docks/playback_dock.h>
#include <docks/node_tree_dock.h>
#include <docks/node_edit_dock.h>
#include "main_window.h"
#include "ui_main_window.h"

using namespace fmt::literals;

namespace studio {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>()),
    error_box(std::make_unique<QErrorMessage>()),
    context(std::make_shared<core::Context>())
{
    ui->setupUi(this);

    connect(ui->action_new, SIGNAL(triggered()), this, SLOT(new_document()));
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_reload, SIGNAL(triggered()), this, SLOT(reload()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_save_as, SIGNAL(triggered()), this, SLOT(save_as()));

    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(quit()));

    connect(ui->action_tool_mouse, SIGNAL(triggered()), this, SLOT(tool_mouse()));
    connect(ui->action_tool_zoom, SIGNAL(triggered()), this, SLOT(tool_zoom()));

    connect(ui->action_render, SIGNAL(triggered()), this, SLOT(render()));
    connect(ui->action_redraw, SIGNAL(triggered()), this, SLOT(redraw()));
    connect(ui->action_extra_style, SIGNAL(toggled(bool)), this, SLOT(toggle_extra_style(bool)));

    connect(ui->action_time_dock, SIGNAL(triggered()), this, SLOT(add_time_dock()));
    connect(ui->action_playback_dock, SIGNAL(triggered()), this, SLOT(add_playback_dock()));
    connect(ui->action_node_tree_dock, SIGNAL(triggered()), this, SLOT(add_node_tree_dock()));

    add_playback_dock();
    add_time_dock();
    add_node_tree_dock();
    add_node_edit_dock();
}

MainWindow::~MainWindow() {
}

void MainWindow::new_document() {
    document = std::make_shared<core::Document>();
    set_context(document->get_default_context());
}

void MainWindow::open() {
    auto fname_qt = QFileDialog::getOpenFileName(this, "Open", "", "RainyNite file (*.rnite)(*.rnite);;Svg paths (*.svgpaths)(*.svgpaths);;All files(*)");
    fname = fname_qt.toStdString();
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
        set_context(document->get_default_context());
        in.close();
        return;
    } catch (std::exception const& ex) {
        auto msg = QString::fromStdString("Uncaught exception in JSON filter:\n{}"_format(ex.what()));
        qDebug() << msg;
    } catch (...) {
        qDebug() << "Unknown error while trying to open document via JSON filter";
    }
    try {
        auto reader = core::filters::SvgPathReader();
        std::ifstream in(fname);
        document = reader.read_document(in);
        set_context(document->get_default_context());
        in.close();
    } catch (std::exception const& ex) {
        auto msg = QString::fromStdString("Uncaught exception while opening document:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while opening document";
    }
}

void MainWindow::save_as() {
    auto fname_qt = QFileDialog::getSaveFileName(this, "Save", "", "RainyNite file (*.rnite)(*.rnite);;All files(*)");
    fname = fname_qt.toStdString();
    if (!fname.empty())
        save();
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
        auto msg = QString::fromStdString("Uncaught exception while saving document:\n{}"_format(ex.what()));
        qDebug() << msg;
        error_box->showMessage(msg);
    } catch (...) {
        qDebug() << "Unknown error while saving document";
    }
}

void MainWindow::render() {
    if (context && context->get_document()) {
        auto rsettings = core::renderers::SvgRendererSettings();
        rsettings.render_pngs = true;
        rsettings.extra_style = extra_style;
        context->mod_render_settings() = rsettings;
        auto renderer = std::make_shared<core::renderers::SvgRenderer>();
        if (render_thread.joinable())
            render_thread.join();
        auto ctx = *context;
        render_thread = std::thread([renderer, ctx]() {
            try {
                renderer->render(ctx);
            } catch (std::exception const& ex) {
                auto msg = QString::fromStdString("Uncaught exception while rendering:\n{}"_format(ex.what()));
                qDebug() << msg;
            }
        });
    }
}

void MainWindow::redraw() {
    set_mainarea_image("renders/{:.3f}.png"_format(context->get_time().get_seconds()));
}

void MainWindow::toggle_extra_style(bool checked) {
    extra_style = checked;
}

void MainWindow::tool_mouse() {
    qDebug() << "mouse selected";
}

void MainWindow::tool_zoom() {
    qDebug() << "zoom selected";
}

void MainWindow::about() {
    QMessageBox::information(this, "About RainyNite",
        QString::fromStdString(
            "RainyNite version {} \"{}\""_format(
                RAINYNITE_STUDIO_VERSION, RAINYNITE_STUDIO_CODENAME
            )
        )
    );
}

void MainWindow::quit() {
    if (render_thread.joinable())
        render_thread.join();
    QApplication::quit();
}

void MainWindow::set_mainarea_image(std::string const& fname) {
    QPixmap pixmap;
    pixmap.load(QString::fromStdString(fname));
    ui->canvas->set_main_image(pixmap);
}

void MainWindow::add_time_dock() {
    auto dock = new TimeDock(context, this);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::add_playback_dock() {
    auto dock = new PlaybackDock(context, this);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::add_node_tree_dock() {
    auto dock = new NodeTreeDock(context, this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::add_node_edit_dock() {
    auto dock = new NodeEditDock(context, this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::set_context(std::shared_ptr<core::Context> context_) {
    context = context_;
    context->changed_time.connect([this](core::Time time){
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
    context->set_active_node(node);
}

}
