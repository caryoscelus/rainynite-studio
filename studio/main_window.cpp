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

#include <core/document.h>
#include <core/filters/svg_path_reader.h>

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
    auto reader = core::filters::SvgPathReader();
    std::ifstream in(fname);
    auto document = reader.read_document(in);
    qDebug() << document->keyframe_amount();
    in.close();
}

void MainWindow::quit() {
    QApplication::quit();
}

}
