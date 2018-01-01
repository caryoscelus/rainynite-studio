/*  default.cpp - default tool
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

#include "base.h"

namespace rainynite::studio::tools {

class Default : public Base {
public:
    string icon() const override {
        return "cursor-arrow";
    }

    string name() const override {
        return global_name();
    }

public:
    static string global_name() {
        return "Default";
    }
};

} // namespace rainynite::studio::tools

namespace rainynite::studio {
REGISTER_CANVAS_TOOL(tools::Default, Canvas, 0x00);
}
