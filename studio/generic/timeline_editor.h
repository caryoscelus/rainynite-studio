/*
 *  timeline_editor.h - timeline editing widgets
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

#ifndef __STUDIO__GENERIC__TIMELINE_EDITOR_H__043F26E4
#define __STUDIO__GENERIC__TIMELINE_EDITOR_H__043F26E4

#include <typeindex>
#include <memory>

#include <core/class_init.h>

/**
 * TODO: merge generic code with CanvasEditor
 */

namespace core {
class AbstractValue;
}

namespace studio {

class TimelineArea;

class TimelineEditor {
public:
    virtual ~TimelineEditor() = default;
public:
    virtual void set_canvas(TimelineArea* canvas_) {
        canvas = canvas_;
    }
    inline TimelineArea* get_canvas() const {
        return canvas;
    }
public:
    /// Set editor preferred vertical position and size
    virtual void set_position_hint(int /*y*/, int /*height*/) {
    }
private:
    TimelineArea* canvas = nullptr;
};

class TimelineEditorFactory {
public:
    virtual std::unique_ptr<TimelineEditor> operator()() const = 0;
};

// TODO: fix this mess
TimelineEditor* add_timeline_editor(TimelineArea& canvas, TimelineEditorFactory const& factory);
TimelineEditor* add_timeline_named_editor(TimelineArea& canvas, std::string const& name);
TimelineEditor* add_timeline_node_editor(TimelineArea& canvas, std::shared_ptr<core::AbstractValue> node);

} // namespace studio

#define REGISTER_TIMELINE_EDITOR(Name, Editor) \
class Name##Factory : \
    public TimelineEditorFactory, \
    private class_init::StringRegistered<Name##Factory, TimelineEditorFactory> \
{ \
public: \
    std::unique_ptr<TimelineEditor> operator()() const override { \
        return std::make_unique<Editor>(); \
    } \
public: \
    static std::string name() { \
        return #Name; \
    } \
}

#define REGISTER_TIMELINE_NODE_EDITOR(Name, Type, Editor) \
class Name##Factory : \
    public TimelineEditorFactory, \
    private class_init::Registered<Name##Factory, Type, TimelineEditorFactory>, \
    private class_init::StringRegistered<Name##Factory, TimelineEditorFactory> \
{ \
public: \
    std::unique_ptr<TimelineEditor> operator()() const override { \
        return std::make_unique<Editor>(); \
    } \
public: \
    static std::string name() { \
        return #Name; \
    } \
}

#endif
