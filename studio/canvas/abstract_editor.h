/*  abstract_editor.h - abstract canvas editor
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

#ifndef STUDIO_CANVAS_ABSTRACT_EDITOR_H_78E5E734_53ED_511D_B6F7_AD0003B70709
#define STUDIO_CANVAS_ABSTRACT_EDITOR_H_78E5E734_53ED_511D_B6F7_AD0003B70709

#include <core/std/vector.h>
#include <core/abstract_factory.h>
#include <core/util/class_init.h>
#include <core/node_tree/node_tree.h>

#include "attachable.h"

namespace rainynite::core {
class AbstractValue;
}

namespace rainynite::studio {

class AbstractCanvasEditor : public CanvasAttachable {
};

struct AbstractAbstractCanvasEditorFactory : public AbstractFactory<AbstractCanvasEditor> {
};

template <class CanvasT>
struct AbstractCanvasEditorFactory : public AbstractAbstractCanvasEditorFactory {
};

template <class CanvasT, class EditorT>
struct AbstractCanvasEditorFactoryI : public AbstractCanvasEditorFactory<CanvasT>
{
    unique_ptr<AbstractCanvasEditor> operator()() const override {
        return make_unique<EditorT>();
    }
};

template <class CanvasT, class EditorT, class Type>
struct AbstractCanvasEditorFactoryInstance :
    public AbstractCanvasEditorFactoryI<CanvasT, EditorT>,
    private class_init::Registered<
        AbstractCanvasEditorFactoryInstance<CanvasT, EditorT, Type>,
        Type,
        AbstractCanvasEditorFactory<CanvasT>
    >
{
};

#define REGISTER_CANVAS_EDITOR(CanvasT, EditorT, Type) \
template struct AbstractCanvasEditorFactoryInstance<CanvasT, EditorT, Type>

#define REGISTER_CANVAS_EDITOR_NAME(CanvasT, EditorT, Name) \
struct Name##AbstractCanvasEditorNameInfoInstance : \
    public AbstractCanvasEditorFactoryI<CanvasT, EditorT>, \
    private class_init::StringRegistered< \
        Name##AbstractCanvasEditorNameInfoInstance, \
        AbstractAbstractCanvasEditorFactory \
    > \
{ \
    static string name() { \
        return #Name; \
    } \
}

template <class CanvasT>
unique_ptr<AbstractCanvasEditor> make_canvas_editor(Type type) {
    return class_init::type_info<AbstractCanvasEditorFactory<CanvasT>,unique_ptr<AbstractCanvasEditor>>(type);
}

/// Create & add canvas editor by name
shared_ptr<AbstractCanvasEditor> add_canvas_named_editor(AbstractCanvas& canvas, string const& name);

/// Create & add canvas node editor to canvas
vector<shared_ptr<AbstractCanvasEditor>> add_canvas_node_editor(AbstractCanvas& canvas, core::NodeTree::Index index);

} // namespace rainynite::studio

#endif
