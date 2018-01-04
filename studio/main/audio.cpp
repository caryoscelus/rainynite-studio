/*  audio.cpp - audio player
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

#include <QMediaPlayer>
#include <QFileInfo>

#include <core/node/abstract_node.h>
#include <core/node/abstract_value.h>

#include <util/strings.h>
#include "audio.h"

namespace rainynite::studio {

AudioPlayer::AudioPlayer() :
    player(make_unique<QMediaPlayer>())
{}

AudioPlayer::~AudioPlayer() {
    if (node_connection.connected())
        node_connection.disconnect();
}

void AudioPlayer::set_context(shared_ptr<EditorContext> context) {
    ContextListener::set_context(context);
    // TODO: detect audio node change
    if (node_connection.connected())
        node_connection.disconnect();
    audio_node = nullptr;
    if (auto doc = get_core_context()->get_document_node()) {
        if (auto value = doc->get_property("soundtrack")) {
            node_connection = value->subscribe([this]() {
                update_audio();
            });
            audio_node = abstract_node_cast(value);
        }
    }
    update_audio();
}

void AudioPlayer::update_audio() {
    if (audio_node) {
        auto maybe_fname = audio_node->get_property_value<string>("file_path", get_core_context());
        auto fname = maybe_fname.value_or("");
        if (fname == cached_file)
            return;
        using util::str;
        player->setMedia(QUrl::fromUserInput(
            str(fname),
            QFileInfo(str(get_context()->get_file_name())).absolutePath()
        ));
        cached_file = fname;

        connect_boost(
            get_context()->changed_time(),
            [this](auto time) {
                update(std::move(time));
            }
        );

        connect_boost(
            get_context()->playback_change,
            [this](bool playing) {
                is_playing = playing;
                if (playing)
                    player->play();
                else
                    player->pause();
            }
        );
    }
}

void AudioPlayer::update(core::Time time) {
    player->setPosition(time.get_seconds()*1000);
    // TODO: play one frame sample even when not playing
    if (is_playing)
        player->play();
}

} // namespace rainynite::studio
