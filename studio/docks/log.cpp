/*  docks/log.cpp - global log
 *  Copyright (C) 2018 caryoscelus
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

#include <iostream>
#include <sstream>

#include <QThread>
#include <QTimer>

#include <util/strings.h>
#include "log.h"
#include "ui_log.h"

namespace rainynite::studio {

class LogDock::RedirectStream : public std::streambuf {
public:
    void set_ui_output(QTextEdit* ui_output_) {
        ui_output.reset(ui_output_);
    }

    void set_console_output(observer_ptr<std::streambuf> console_output_) {
        console_output = console_output_;
    }

protected:
    int_type overflow(int_type ch) override {
        if (ch == std::char_traits<char>::eof())
            return ch;
        if (console_output) {
            console_output->sputc(ch);
        }
        char c = ch;
        write_to_ui(string(&c, 1));
        return ch;
    }
    std::streamsize xsputn(char const* line, std::streamsize n) override {
        if (console_output)
            console_output->sputn(line, n);
        write_to_ui(string(line, n));
        return n;
    }

public:
    void flush() {
        auto s = buff.str();
        buff = std::ostringstream();
        if (s.size() > 0) {
            ui_output->append(util::str(std::move(s)));
        }
    }

private:
    void write_to_ui(string&& s) {
        if (QThread::currentThread() == ui_output->thread()) {
            flush();
            ui_output->append(util::str(s));
        } else {
            buff << s;
        }
    }

    observer_ptr<QTextEdit> ui_output;
    observer_ptr<std::streambuf> console_output;
    std::ostringstream buff;
};

observer_ptr<LogDock::RedirectStream> global_buff;

void handler(QtMsgType, QMessageLogContext const&, QString const& s) {
    if (!global_buff)
        return;
    string cs = util::str(s)+"\n";
    global_buff->sputn(cs.c_str(), cs.size());
}

LogDock::LogDock(QWidget* parent) :
    DockWidget(parent),
    ui(make_unique<Ui::LogDock>()),
    buff(make_unique<RedirectStream>()),
    timer(make_unique<QTimer>())
{
    ui->setupUi(this);

    global_buff.reset(buff.get());

    old_cout.reset(std::cout.rdbuf(buff.get()));
    old_cerr.reset(std::cerr.rdbuf(buff.get()));

    qInstallMessageHandler(&handler);

    buff->set_ui_output(ui->text);
    buff->set_console_output(old_cout);

    connect(timer.get(), &QTimer::timeout, [this]() {
        buff->flush();
    });
    timer->start(1000); // force log update each second
}

LogDock::~LogDock() {
    std::cout.rdbuf(old_cout.get());
    std::cerr.rdbuf(old_cerr.get());
    qInstallMessageHandler(0);
    global_buff.reset();
}

} // namespace rainynite::studio
