/*  renderer.h - renderer
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

#ifndef STUDIO_MAIN_RENDERER_H_39419D23_FC77_52F5_BAE0_212BEBF4F7AD
#define STUDIO_MAIN_RENDERER_H_39419D23_FC77_52F5_BAE0_212BEBF4F7AD

#include <thread>
#include <mutex>
#include <queue>

#include <QObject>

#include <generic/context_listener.h>

namespace rainynite::core::renderers {
class SvgRenderer;
}

namespace rainynite::studio {

class Canvas;

class Renderer : public QObject, public ContextListener {
    Q_OBJECT

public:
    Renderer(Canvas* canvas_);
    virtual ~Renderer();

    void setup_renderer();
    void render_period(core::TimePeriod const& period);
    void set_fname(string fname_);

    void set_context(shared_ptr<EditorContext> context_) override;

public Q_SLOTS:
    /// Render active period
    void render();

    /// Render current frame
    void render_frame();

    void stop_render();

    /// Redraw canvas using render result
    void redraw();

    void toggle_extra_style(bool checked);

    void toggle_auto_redraw(bool checked);

Q_SIGNALS:
    void redraw_signal();

private:
    void set_mainarea_image(string const& fname);
    void quit();

private:
    Canvas* canvas;

    string fname;
    bool extra_style = true;
    bool auto_redraw = true;

    std::thread render_thread;
    shared_ptr<core::renderers::SvgRenderer> renderer;
    std::queue<core::Context> renderer_queue;
    std::mutex renderer_mutex;
    bool renderer_quit = false;
    bool renderer_restart = false;
};

} // namespace rainynite::studio

#endif
