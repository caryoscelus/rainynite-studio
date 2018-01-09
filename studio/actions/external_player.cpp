/*  external_player.cpp - play animation in external player
 *  Copyright (C) 2017-2018 caryoscelus
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

#include <cstdlib>
#include <thread>

#include <fmt/format.h>

#include <core/document.h>
#include <core/node/abstract_node.h>
#include <core/node/abstract_value.h>

#include <generic/action.h>

using namespace fmt::literals;

namespace rainynite::studio::actions {

/**
 * Launch external player (or, perhaps, other command on rendered frames)
 *
 * TODO: configuration: UI & store
 */
class LaunchExternalPlayer : public UiAction, public ContextListener {
    ACTION_NAME("Launch external player")
public:
    void process() override {
        if (auto ctx = get_core_context()) {
            string audio_file;
            if (auto audio_node = abstract_node_cast(ctx->get_document_node()->get_property("soundtrack")))
                audio_file = audio_node->get_property_value<string>("file_path", ctx).value_or("");
            auto command = fmt::format(
                player_command(),
                "audio_command"_a=audio_command(audio_file),
                "lst_file"_a="renders/rendered.lst",
                "fps"_a=ctx->get_fps()
            );
            std::thread t([command]() {
                std::system(command.c_str());
            });
            t.detach();
        }
    }
protected:
    virtual string player_command() const = 0;
    virtual string audio_command(string const& fname) const = 0;
};

class LaunchMpv : public LaunchExternalPlayer, REGISTERED_ACTION(LaunchMpv) {
    ACTION_NAME("Launch external player (mpv)")
protected:
    string player_command() const override {
        return "mpv -loop {audio_command} -mf-fps {fps} mf://@{lst_file}";
    }
    string audio_command(string const& fname) const override {
        if (fname.empty())
            return "";
        return "-audio-file '"+fname+"'";
    }
};

class FfmpegToWebm : public LaunchExternalPlayer, REGISTERED_ACTION(FfmpegToWebm) {
    ACTION_NAME("Encode render results to webm (ffmpeg)")
protected:
    string player_command() const override {
        return R"(cat {lst_file} | sed -e "s/^/file '/" -e "s/$/'/" > renders/rendered_ffmpeg.lst && ffmpeg -f concat -safe 0 -r {fps} -i renders/rendered_ffmpeg.lst {audio_command} result.webm)";
    }
    string audio_command(string const& fname) const override {
        if (fname.empty())
            return "";
        return "-i '"+fname+"'";
    }
};

} // namespace rainynite::studio::actions
