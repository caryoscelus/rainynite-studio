/*  waveform.cpp - waveform timeline "widget"
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

#include <thread>
#include <iostream>

#include <sys/wait.h>

#include <fmt/format.h>

#include <core/audio.h>
#include <core/os/fork_pipe.h>

#include <generic/node_editor.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include <util/strings.h>

using namespace fmt::literals;

namespace rainynite::studio {

class WaveformDisplay :
    public TimelineEditor,
    public NodeEditor
{
public:
    virtual ~WaveformDisplay() {
        if (read_thread.joinable())
            read_thread.detach();
    }

    void setup_canvas() override {
        node_update();
    }

    void node_update() override {
        if (auto canvas = get_canvas()) {
            if (auto file_path = get_path(); !file_path.empty()) {
                if (file_path != cached_path) {
                    generate_waveform(file_path);
                }
            } else {
                canvas->set_background_image({});
            }
        }
    }

private:
    string get_path() {
        if (auto audio_value = get_node_as<core::Audio>()) {
            auto node = dynamic_cast<core::AbstractNode*>(audio_value.get());
            if (node == nullptr)
                return "";
            try {
                auto path_node = node->get_property_as<string>("file_path");
                if (path_node == nullptr)
                    return "";
                return path_node->value(get_core_context());
            } catch (core::NodeAccessError) {
                return "";
            }
        } else {
            return "";
        }
    }

    void generate_waveform(string const& file_path) {
        FILE* pipe_input; // unused
        FILE* pipe_output;
#warning "it isn't very safe to pass unchecked string to external program"
        auto pid = fork_pipe(pipe_input, pipe_output, {"/usr/bin/env", "ffprobe", "-i", file_path, "-show_entries", "format=duration", "-v", "quiet", "-of", "csv=p=0"});
        cached_path = file_path;
        read_thread = std::thread([this, pipe_output, pid]() {
            int status;
            waitpid(pid, &status, 0);
            if (status != 0)
                return;
            double duration;
            if (fscanf(pipe_output, "%lf", &duration) < 1)
                return;
            if (duration <= 0)
                return;
            generate_waveform_with_duration(duration);
        });
    }
    void generate_waveform_with_duration(double duration) {
        FILE* pipe_input; // unused
        FILE* pipe_output; // unused
#warning "it isn't very safe to pass unchecked string to external program"
        auto pid = fork_pipe(pipe_input, pipe_output, {"/usr/bin/env", "ffmpeg", "-i", cached_path, "-filter_complex", "showwavespic=s={}x{}:colors=black|gray"_format(int(duration*pixels_per_second), 80), "-frames:v", "1", "-y", cached_path+".png"});
        if (read_thread.joinable())
            read_thread.detach();
        read_thread = std::thread([this, pipe_output, pid]() {
            int status;
            waitpid(pid, &status, 0);
            if (status == 0)
                load_waveform();
        });
    }
    void load_waveform() {
        QPixmap pixmap;
        pixmap.load(util::str(cached_path+".png"));
        get_canvas()->set_background_image(pixmap);
        get_canvas()->set_bg_transform(QTransform().scale(1.0/pixels_per_second, 1));
    }

private:
    string cached_path;
    const double pixels_per_second = 8;
    std::thread read_thread;
};

REGISTER_CANVAS_EDITOR(TimelineArea, WaveformDisplay, core::Audio);

}
