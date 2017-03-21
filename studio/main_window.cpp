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

#include <QFileDialog>
#include <QDebug>

#include <core/time.h>
#include <core/document.h>
#include <core/filters/svg_path_reader.h>
#include <core/renderers/svg_renderer.h>

#include "main_window.h"
#include "ui_main_window.h"

namespace studio {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(quit()));
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
        context = document->get_default_context();
        qDebug() << document->keyframe_amount();
        in.close();
    } catch (...) {
        qDebug() << "Error while opening document";
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
            set_mainarea_image("renders/0.000.png");
        });
        if (render_thread.joinable())
            render_thread.join();
        auto ctx = *context;
        render_thread = std::thread([renderer, ctx]() {
            renderer->render(ctx);
        });
    }
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

}
