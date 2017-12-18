/*  import.cpp - "import" actions
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

#include <QFileDialog>

#include <core/document.h>
#include <core/node_info.h>
#include <core/action_stack.h>
#include <core/actions/list.h>

#include <generic/action.h>
#include <util/strings.h>
#include "process_node.h"

namespace rainynite::studio::actions {

struct ImportFilesTag {};

class FillStringList : public ProcessNode<string, ImportFilesTag> {
public:
    bool accept(core::AbstractValue const& node) override {
        return node.get_type() == typeid(vector<string>);
    }
    void feed(string const& s) override {
        auto string_node = core::make_value<string>(std::move(s));
        get_context()->action_stack()->emplace<core::actions::ListPush>(list_cast(get_node()), string_node);
    }
};

REGISTER_PROCESS_NODE(FillStringList, string, ImportFilesTag)

class FillRenderableList : public ProcessNode<string, ImportFilesTag> {
public:
    bool accept(core::AbstractValue const& node) override {
        return node.get_type() == typeid(vector<core::Renderable>);
    }
    void feed(string const& s) override {
        auto string_node = core::make_value<string>(std::move(s));
        auto image_node = core::make_node_with_name<core::AbstractNode>("Image");
        image_node->set_property("file_path", string_node);
        // TODO: set size
        get_context()->action_stack()->emplace<core::actions::ListPush>(list_cast(get_node()), abstract_value_cast(image_node));
    }
};

REGISTER_PROCESS_NODE(FillRenderableList, string, ImportFilesTag)

/**
 * Import file paths
 */
class ImportRasterLayers : CONTEXT_ACTION(ImportRasterLayers) {
    ACTION_NAME("Import raster layers")
public:
    using Processor = ProcessNode<string, ImportFilesTag>;

    void process() override {
        if (auto target = find_appropriate_target()) {
            // TODO: filter images
            auto strings = QFileDialog::getOpenFileNames(nullptr, "Import layers");
            for (auto const& s : strings)
                target->feed(util::str(s));
        } else {
            // report error
        }
    }

    unique_ptr<Processor> find_appropriate_target() {
        // TODO: more generic
        if (auto ctx = get_context()) {
            if (auto active_node = ctx->get_active_node()) {
                if (auto r = check_target(active_node))
                    return r;
                if (auto list = list_cast(active_node)) {
                    for (auto child : list->get_links()) {
                        if (auto r = check_target(child))
                            return r;
                    }
                }
            }
            if (auto doc = get_core_context()->get_document()) {
                if (auto root = list_cast(doc->get_property("root"))) {
                    for (auto child : root->get_links()) {
                        if (auto r = check_target(child))
                            return r;
                    }
                }
            }
        }
        return {};
    }

    unique_ptr<Processor> check_target(shared_ptr<core::AbstractValue> node) {
        using namespace class_init;
        for (auto factory : class_list_registry<AbstractFactory<Processor>>()) {
            auto r = (*factory)();
            r->set_context(get_context());
            if (r->set_node(node))
                return r;
        }
        return nullptr;
    }
};

} // namespace rainynite::studio::actions
