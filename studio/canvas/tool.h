/*  tool.h - abstract canvas tool
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

#ifndef STUDIO_CANVAS_TOOL_H_DE911363_8896_5B67_A000_6B06587D552A
#define STUDIO_CANVAS_TOOL_H_DE911363_8896_5B67_A000_6B06587D552A

#include <QObject>

#include <core/abstract_factory.h>
#include <core/class_init.h>

#include "attachable.h"

namespace rainynite::studio {

class CanvasTool : public QObject, public CanvasAttachable {
public:
    virtual bool canvas_event(QEvent* event) = 0;
    virtual string icon() const = 0;
    virtual string name() const = 0;

    bool eventFilter(QObject* target, QEvent* event) override;
};

template <class CanvasT>
struct CanvasToolFactory : public AbstractFactory<CanvasTool> {
    virtual size_t priority() const = 0;
};

// TODO: make more generic
template <class CanvasT, class ToolT, size_t P>
struct CanvasToolFactoryInstance :
    public CanvasToolFactory<CanvasT>,
    private class_init::ListRegistered<
        CanvasToolFactoryInstance<CanvasT, ToolT, P>,
        CanvasToolFactory<CanvasT>
    >
{
    unique_ptr<CanvasTool> operator()() const override {
        return make_unique<ToolT>();
    }
    size_t priority() const override {
        return P;
    }
};

#define REGISTER_CANVAS_TOOL(Tool, CanvasT, P) \
template struct CanvasToolFactoryInstance<CanvasT, Tool, P>

template <class CanvasT>
vector<CanvasToolFactory<CanvasT>*> const& get_canvas_tools() {
    return class_init::class_list_registry<CanvasToolFactory<CanvasT>>();
}

} // namespace rainynite::studio

#endif
