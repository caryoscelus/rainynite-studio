/*  import_soundtrack.cpp - import soundtrack action
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

#include <core/node_info/node_info.h>
#include <core/node/make.h>
#include <core/action_stack.h>
#include <core/actions/change_link.h>

#include "process_node.h"
#include "import.h"

namespace rainynite::studio::actions {

struct ImportSoundTag {};

using ImportSoundProcessor = ProcessNode<string, ImportSoundTag>;

class SetSoundtrack : public ProcessNode<string, ImportSoundTag> {
public:
    bool accept(core::NodeTree::Index node) const override {
        auto tree = get_context()->tree();
        return node_name(*tree->get_node(node)) == "AudioFromFile";
    }
    void feed(string const& s) override {
        auto string_node = core::make_value<string>(std::move(s));
        auto ctx = get_context();
        ctx->action_stack()->emplace<core::actions::SetProperty>(ctx->tree(), get_node_index(), "file_path", string_node);
    }
};

REGISTER_PROCESS_NODE(SetSoundtrack, string, ImportSoundTag)

/**
 * Import file paths as Image layers
 */
class ImportSoundtrack :
    public ImportFiles<ImportSoundProcessor>,
    REGISTERED_ACTION(ImportSoundtrack)
{
    ACTION_NAME("Import soundtrack")
};

} // namespace rainynite::studio::actions
