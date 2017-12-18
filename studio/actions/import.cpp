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

template <typename Processor>
class FindTargetNode {
public:
    unique_ptr<Processor> find_appropriate_target(shared_ptr<EditorContext> ctx) {
        if (auto active_node = ctx->get_active_node()) {
            if (auto r = check_target(active_node, ctx))
                return r;
            if (auto list = list_cast(active_node)) {
                for (auto child : list->get_links()) {
                    if (auto r = check_target(child, ctx))
                        return r;
                }
            }
        }
        if (auto doc = ctx->get_context()->get_document()) {
            if (auto root = list_cast(doc->get_property("root"))) {
                for (auto child : root->get_links()) {
                    if (auto r = check_target(child, ctx))
                        return r;
                }
            }
        }
        return {};
    }

    unique_ptr<Processor> check_target(shared_ptr<core::AbstractValue> node, shared_ptr<EditorContext> ctx) const {
        using namespace class_init;
        for (auto factory : class_list_registry<AbstractFactory<Processor>>()) {
            auto r = (*factory)();
            r->set_context(ctx);
            if (r->set_node(node))
                return r;
        }
        return nullptr;
    }
};

using ImportFilesProcessor = ProcessNode<string, ImportFilesTag>;

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
            if (auto target = this->find_appropriate_target(ctx)) {
                // TODO: filter images
                auto strings = QFileDialog::getOpenFileNames(nullptr, "Import layers");
                for (auto const& s : strings)
                    target->feed(util::str(s));
            } else {
                // report error
            }
        } else {
            // report error
        }
    }
};

class ImportRasterLayers :
    public ImportFiles<ImportFilesProcessor>,
    REGISTERED_ACTION(ImportRasterLayers)
{
    ACTION_NAME("Import raster layers")
};

} // namespace rainynite::studio::actions
