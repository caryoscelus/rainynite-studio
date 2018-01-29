/*  import.h - "import" actions base
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

#ifndef STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A
#define STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A

#include <QFileDialog>

#include <core/node/abstract_list.h>
#include <core/node/abstract_node.h>
#include <core/util/nullptr.h>

#include <generic/action.h>
#include <generic/process_node.h>
#include <util/strings.h>

namespace rainynite::studio::actions {

struct ImportError : public std::runtime_error {
    ImportError(string const& msg) :
        std::runtime_error(msg)
    {}
};

/**
 * Import file paths
 */
template <typename Processor>
class ImportFiles :
    public UiAction,
    public ContextListener,
    FindTargetNode<Processor>
{
public:
    void process() override {
        if (auto ctx = get_context()) {
            if (auto target = this->find_appropriate_target(*no_null(ctx->tree()), ctx)) {
                // TODO: filter images
                auto strings = QFileDialog::getOpenFileNames(nullptr, "Import layers");
                target->start_list();
                for (auto const& s : strings)
                    target->feed(util::str(s));
                target->end_list();
            } else {
                throw ImportError("Cannot find suitable target node.");
            }
        } else {
            throw ImportError("No context! This is a bug.");
        }
    }
};

} // namespace rainynite::studio::actions

#endif
