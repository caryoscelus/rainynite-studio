/*  audio.h - audio player
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

#ifndef STUDIO_MAIN_AUDIO_H_0BD8ABBD_AB43_58DF_BCCA_172630D4F535
#define STUDIO_MAIN_AUDIO_H_0BD8ABBD_AB43_58DF_BCCA_172630D4F535

#include <core/std/string.h>

#include <generic/context_listener.h>

class QMediaPlayer;

namespace rainynite::studio {

class AudioPlayer : public ContextListener {
public:
    AudioPlayer();
    virtual ~AudioPlayer();

    void set_context(shared_ptr<EditorContext> context) override;

    void update(core::Time time);

private:
    string cached_file;

    unique_ptr<QMediaPlayer> player;
};

} // namespace rainynite::studio

#endif
