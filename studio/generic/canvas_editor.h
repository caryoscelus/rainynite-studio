/*
 *  canvas_editor.h - on-canvas value edit/display "widgets"
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

#ifndef __STUDIO__CANVAS_EDITOR_H__F294C054
#define __STUDIO__CANVAS_EDITOR_H__F294C054

#include <typeindex>
#include <memory>

namespace core {
class AbstractValue;
}

namespace studio {

class Canvas;

class CanvasEditor {
public:
    virtual ~CanvasEditor() = default;
public:
    virtual void set_canvas(Canvas* canvas_);
    inline Canvas* get_canvas() const {
        return canvas;
    }
private:
    Canvas* canvas = nullptr;
};

class CanvasEditorFactory {
public:
    virtual std::unique_ptr<CanvasEditor> operator()() const = 0;
};

void add_canvas_editor(Canvas& canvas, std::shared_ptr<core::AbstractValue> node);

} // namespace studio

#define REGISTER_CANVAS_EDITOR(Name, Type, Editor) \
class Name##Factory : \
    public CanvasEditorFactory, \
    class_init::Registered<Name##Factory, Type, CanvasEditorFactory> \
{ \
public: \
    virtual std::unique_ptr<CanvasEditor> operator()() const override { \
        return std::make_unique<Editor>(); \
    } \
}

#endif
