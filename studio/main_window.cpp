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
#include <QDebug>

#include <core/document.h>
#include <core/filters/svg_path_reader.h>
#include <core/renderers/svg_renderer.h>

#include "time_dock.h"
#include "playback_dock.h"
#include "node_tree_dock.h"
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
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->action_render, SIGNAL(triggered()), this, SLOT(render()));
    connect(ui->action_redraw, SIGNAL(triggered()), this, SLOT(redraw()));
    connect(ui->action_time_dock, SIGNAL(triggered()), this, SLOT(add_time_dock()));
    connect(ui->action_playback_dock, SIGNAL(triggered()), this, SLOT(add_playback_dock()));
    connect(ui->action_node_tree_dock, SIGNAL(triggered()), this, SLOT(add_node_tree_dock()));
    add_playback_dock();
    add_time_dock();
    add_node_tree_dock();
}

MainWindow::~MainWindow() {
}

void MainWindow::open() {
    auto fname_qt = QFileDialog::getOpenFileName(this, "Open", "", "Svg paths (*.svgpaths)(*.svgpaths);;All files(*)");
    auto fname = fname_qt.toStdString();
    qDebug() << fname_qt;
    if (fname.empty())
        return;
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
    render();
}

void MainWindow::render() {
    if (context && context->get_document()) {
        auto rsettings = core::renderers::SvgRendererSettings();
        rsettings.render_pngs = true;
        context->mod_render_settings() = rsettings;
        auto renderer = std::make_shared<core::renderers::SvgRenderer>();
        renderer->finished_frame().connect([this](core::Time frame_time) {
            redraw();
        });
        if (render_thread.joinable())
            render_thread.join();
        auto ctx = *context;
        render_thread = std::thread([renderer, ctx]() {
            renderer->render(ctx);
        });
    }
}

void MainWindow::redraw() {
    set_mainarea_image("renders/{:.3f}.png"_format(context->get_time().get_seconds()));
}

void MainWindow::quit() {
    if (render_thread.joinable())
        render_thread.join();
    QApplication::quit();
}

void MainWindow::set_mainarea_image(std::string const& fname) {
    QPixmap pixmap;
    pixmap.load(QString::fromStdString(fname));
    ui->image->setPixmap(pixmap);
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

void MainWindow::set_context(std::shared_ptr<core::Context> context_) {
    context = context_;
    context->changed_time.connect([this](core::Time time){
        redraw();
    });
    for (auto dock : findChildren<QDockWidget*>()) {
        if (auto ctx_dock = dynamic_cast<ContextListener*>(dock))
            ctx_dock->set_context(context_);
        if (dock->metaObject()->indexOfSignal("activated(core::AbstractReference)") != -1)
            connect(dock, SIGNAL(activated(core::AbstractReference)), this, SLOT(activate()));
    }
}

void MainWindow::activate() {
    qDebug() << "xxx [ * * * ] xxx";
}

}
