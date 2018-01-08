/*  import.cpp - "import" actions
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
#include <core/actions/list.h>
#include <core/renderable.h>

#include "process_node.h"
#include "import.h"

namespace rainynite::studio::actions {

struct ImportFilesTag {};
struct ImportFramesTag {};
struct ImportSvgLayersTag {};

using ImportFilesProcessor = ProcessNode<string, ImportFilesTag>;
using ImportFramesProcessor = ProcessNode<string, ImportFramesTag>;
using ImportSvgLayersProcessor = ProcessNode<string, ImportSvgLayersTag>;

class FillStringList : public ProcessNode<string, ImportFilesTag> {
public:
    bool accept(core::AbstractValue const& node) const override {
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
    bool accept(core::AbstractValue const& node) const override {
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

class AddFramedAnimationToRenderableList : public ProcessNode<string, ImportFramesTag> {
public:
    bool accept(core::AbstractValue const& node) const override {
        return node.get_type() == typeid(vector<core::Renderable>);
    }
    void start_list() override {
        file_name_list_node = core::make_node_with_name<core::AbstractListLinked>("List/String");
    }
    void feed(string const& s) override {
        push_value(file_name_list_node, std::move(s));
    }
    void end_list() override {
        auto image_node = core::make_node_with_name<core::AbstractNode>("Image");
        auto file_name_node = core::make_node_with_name<core::AbstractNode>("ListElement/String");
        auto linear_node = core::make_node_with_name<core::AbstractNode>("Linear");

        linear_node->set_property("speed", core::make_value<double>(get_core_context()->get_fps()));
        file_name_node->set_property("source", abstract_value_cast(file_name_list_node));
        file_name_node->set_property("n", abstract_value_cast(linear_node));
        image_node->set_property("file_path", abstract_value_cast(file_name_node));

        get_context()->action_stack()->emplace<core::actions::ListPush>(list_cast(get_node()), abstract_value_cast(image_node));
    }
private:
    shared_ptr<core::AbstractListLinked> file_name_list_node;
};

REGISTER_PROCESS_NODE(AddFramedAnimationToRenderableList, string, ImportFramesTag)

class FillRenderableListWithSvgs : public ProcessNode<string, ImportSvgLayersTag> {
public:
    bool accept(core::AbstractValue const& node) const override {
        return node.get_type() == typeid(vector<core::Renderable>);
    }
    void feed(string const& s) override {
        auto string_node = core::make_value<string>(std::move(s));
        auto image_node = core::make_node_with_name<core::AbstractNode>("EmbedSvg");
        image_node->set_property("file_path", string_node);
        get_context()->action_stack()->emplace<core::actions::ListPush>(list_cast(get_node()), abstract_value_cast(image_node));
    }
};

REGISTER_PROCESS_NODE(FillRenderableListWithSvgs, string, ImportSvgLayersTag)

/**
 * Import file paths as Image layers
 */
class ImportRasterLayers :
    public ImportFiles<ImportFilesProcessor>,
    REGISTERED_ACTION(ImportRasterLayers)
{
    ACTION_NAME("Import raster layers")
};

/**
 * Import file paths as frames
 */
class ImportRasterFrames :
    public ImportFiles<ImportFramesProcessor>,
    REGISTERED_ACTION(ImportRasterFrames)
{
    ACTION_NAME("Import raster frames")
};

/**
 * Import svgs
 */
class ImportSvgLayers :
    public ImportFiles<ImportSvgLayersProcessor>,
    REGISTERED_ACTION(ImportSvgLayers)
{
    ACTION_NAME("Import svg layers")
};

} // namespace rainynite::studio::actions
