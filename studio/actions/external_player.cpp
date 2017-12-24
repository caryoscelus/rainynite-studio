/*  external_player.cpp - play animation in external player
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

#include <cstdlib>
#include <thread>

#include <fmt/format.h>

#include <core/document.h>

#include <generic/action.h>

using namespace fmt::literals;

namespace rainynite::studio::actions {

/**
 * Launch external player (or, perhaps, other command on rendered frames)
 *
 * TODO: configuration: UI & store
 */
class LaunchExternalPlayer : CONTEXT_ACTION(LaunchExternalPlayer) {
    ACTION_NAME("Launch external player")
public:
    void process() override {
        if (auto ctx = get_core_context()) {
            string audio_file;
            if (auto audio_node = core::abstract_node_cast(ctx->get_document()->get_property("soundtrack")))
                audio_file = audio_node->get_property_value<string>("file_path", ctx).value_or("");
            if (audio_file.empty())
                audio_file = "__NOAUDIO__";
            auto command = fmt::format(
                player_command,
                "audio_file"_a=audio_file,
                "lst_file"_a="renders/rendered.lst",
                "fps"_a=ctx->get_fps()
            );
            std::thread t([command]() {
                std::system(command.c_str());
            });
            t.detach();
        }
    }

private:
    string player_command = "mpv -audio-file {audio_file} -mf-fps {fps} mf://@{lst_file}";
};

} // namespace rainynite::studio::actions
