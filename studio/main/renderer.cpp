/*
 *  renderer.cpp - renderer
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

#include <boost/filesystem/path.hpp>

#include <fmt/format.h>

#include <QDebug>
#include <QPixmap>

#include <core/renderers/svg_renderer.h>

#include <util/strings.h>
#include <widgets/canvas.h>
#include "renderer.h"

using namespace fmt::literals;

namespace rainynite::studio {

Renderer::Renderer(Canvas* canvas_) :
    canvas(canvas_)
{
    connect(this, SIGNAL(redraw_signal()), this, SLOT(redraw()));
    setup_renderer();
}

Renderer::~Renderer() {
    quit();
}

void Renderer::set_fname(string fname_) {
    fname = fname_;
}

void Renderer::set_context(shared_ptr<EditorContext> context_) {
    ContextListener::set_context(context_);
    get_context()->changed_time().connect([this](core::Time){
        redraw();
    });
}

void Renderer::setup_renderer() {
    renderer = make_shared<core::renderers::SvgRenderer>();
    render_thread = std::thread([this]() {
        while (!renderer_quit) {
            if (renderer_queue.size() > 0) {
                renderer_mutex.lock();
                auto ctx = std::move(renderer_queue.front());
                renderer_queue.pop();
                renderer_mutex.unlock();

                try {
                    renderer->render(std::move(ctx));
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

void Renderer::render_period(core::TimePeriod const& period) {
    if (get_core_context()->get_document()) {
        auto rsettings = core::renderers::SvgRendererSettings();
        rsettings.render_pngs = true;
        rsettings.keep_alive = true;
        rsettings.extra_style = extra_style;
        rsettings.path = fname;
        get_core_context()->mod_render_settings() = rsettings;
        auto ctx = *get_core_context();
        ctx.set_period(period);

        std::lock_guard<std::mutex> lock(renderer_mutex);
        renderer_queue.push(ctx);
    }
}

void Renderer::render() {
    render_period(get_core_context()->get_period());
}

void Renderer::render_frame() {
    auto time = get_core_context()->get_time();
    auto time_end = time;
    ++time_end;
    render_period({time, time_end});
}

void Renderer::stop_render() {
    renderer->stop();
}

void Renderer::redraw() {
    // TODO: fix this mess
    boost::filesystem::path base_path = fname;
    base_path.remove_filename();
    auto path = base_path/"renders"/"{:.3f}.png"_format(get_core_context()->get_time().get_seconds());
    set_mainarea_image(path.string());
}

void Renderer::toggle_extra_style(bool checked) {
    extra_style = checked;
}

void Renderer::set_mainarea_image(string const& fname) {
    QPixmap pixmap;
    pixmap.load(util::str(fname));
    canvas->set_main_image(pixmap);
}

void Renderer::quit() {
    stop_render();
    if (render_thread.joinable()) {
        renderer_quit = true;
        render_thread.join();
    }
}

} // namespace rainynite::studio
