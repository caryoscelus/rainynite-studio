/*  registry.h - canvas registry & non-template tool/editor add functions
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

#ifndef STUDIO_CANVAS_REGISTRY_H_20662128_B848_5B8E_B4FC_80D315C1A843
#define STUDIO_CANVAS_REGISTRY_H_20662128_B848_5B8E_B4FC_80D315C1A843

#include <core/std/memory.h>
#include <core/std/vector.h>

#include "abstract_canvas.h"
#include "abstract_editor.h"
#include "tool.h"

namespace rainynite::studio {

class CanvasTool;
class AbstractCanvasEditor;

struct CanvasToolsInfo {
    virtual vector<AbstractFactory<CanvasTool>*> operator()() const = 0;
};

struct AbstractCanvasEditorsInfo {
    virtual unique_ptr<AbstractCanvasEditor> operator()(Type type) const = 0;
};

struct AbstractCanvasNodeEditorsInfo {
    virtual unique_ptr<AbstractCanvasEditor> operator()(std::type_index node_type) const = 0;
};

template <class CanvasT>
struct CanvasToolsInfoInstance :
    public CanvasToolsInfo,
    private class_init::Registered<
        CanvasToolsInfoInstance<CanvasT>,
        CanvasT,
        CanvasToolsInfo
    >
{
    vector<AbstractFactory<CanvasTool>*> operator()() const override {
        auto factory_list = get_canvas_tools<CanvasT>();
        vector<AbstractFactory<CanvasTool>*> result;
        std::sort(
            factory_list.begin(),
            factory_list.end(),
            [](auto a, auto b) {
                return a->priority() < b->priority();
            }
        );
        std::transform(
            factory_list.begin(),
            factory_list.end(),
            std::back_inserter(result),
            [](auto const& e) { return e; }
        );
        return result;
    }
};

template <class CanvasT>
struct AbstractCanvasEditorsInfoInstance :
    public AbstractCanvasEditorsInfo,
    private class_init::Registered<
        AbstractCanvasEditorsInfoInstance<CanvasT>,
        CanvasT,
        AbstractCanvasEditorsInfo
    >
{
    unique_ptr<AbstractCanvasEditor> operator()(Type type) const override {
        return make_canvas_editor<CanvasT>(type);
    }
};

template <class CanvasT>
struct AbstractCanvasNodeEditorsInfoInstance :
    public AbstractCanvasNodeEditorsInfo,
    private class_init::Registered<
        AbstractCanvasNodeEditorsInfoInstance<CanvasT>,
        CanvasT,
        AbstractCanvasNodeEditorsInfo
    >
{
    unique_ptr<AbstractCanvasEditor> operator()(std::type_index node_type) const override {
        return make_canvas_node_editor<CanvasT>(node_type);
    }
};

#define REGISTER_CANVAS(CanvasT) \
template struct CanvasToolsInfoInstance<CanvasT>; \
template struct AbstractCanvasEditorsInfoInstance<CanvasT>; \
template struct AbstractCanvasNodeEditorsInfoInstance<CanvasT>

inline vector<AbstractFactory<CanvasTool>*> get_canvas_tools_by_type(Type type) {
    return class_init::type_info<CanvasToolsInfo,vector<AbstractFactory<CanvasTool>*>>(type);
}

inline unique_ptr<AbstractCanvasEditor> make_canvas_editor_for(AbstractCanvas const& canvas, Type type) {
    return class_init::type_info<AbstractCanvasEditorsInfo,unique_ptr<AbstractCanvasEditor>>(typeid(canvas), type);
}

inline unique_ptr<AbstractCanvasEditor> make_canvas_node_editor_for(AbstractCanvas const& canvas, std::type_index node_type) {
    return class_init::type_info<AbstractCanvasNodeEditorsInfo,unique_ptr<AbstractCanvasEditor>>(typeid(canvas), node_type);
}

} // namespace rainynite::studio

#endif
