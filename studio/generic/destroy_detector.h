/*  destroy_detector.h - report object destruction for boost signal system
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

#ifndef STUDIO_GENERIC_DESTROY_DETECTOR_H_DD71BC27_C341_5CF3_8E0B_71C19AE585A2
#define STUDIO_GENERIC_DESTROY_DETECTOR_H_DD71BC27_C341_5CF3_8E0B_71C19AE585A2

#include <core/std/memory.h>

namespace rainynite::studio {

/**
 * A base class that allows tracking connection to boost signals.
 */
class DestroyDetector {
public:
    DestroyDetector() :
        destroy_detector(make_shared<Null>())
    {}

    template <class S, class F>
    void connect_boost(S& signal, F lambda) {
        auto slot = typename S::slot_type(lambda);
        slot.track_foreign(destroy_detector);
        signal.connect(slot);
    }

private:
    struct Null {};
    shared_ptr<Null> const destroy_detector;
};

}

#endif
