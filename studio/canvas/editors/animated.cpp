/*  animated.cpp - context menu for animated-like nodes
 *  Copyright (C) 2018 caryoscelus
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

#include <QMenu>

#include <core/util/nullptr.h>

#include <generic/node_editor.h>
#include <generic/timeline_editor.h>
#include <widgets/timeline_area.h>
#include <util/strings.h>

using namespace fmt::literals;

namespace rainynite::studio {

class AnimatedEditor :
    public TimelineEditor,
    public NodeEditor
{
public:
    virtual bool context_menu(double seconds, QMenu& menu) {
        menu.addAction(QIcon::fromTheme("frame-insert"), "Insert frame", [this, seconds]() {
            if (auto node = get_node()) {
                if (node->can_set_any_at()) {
                    auto ctx = no_null(get_core_context());
                    auto nctx = make_shared<core::Context>(*ctx);
                    nctx->set_seconds(seconds);
                    node->set_any_at(node->get_any(nctx), nctx);
                    // animated nodes add new frames and it's not being reflected in tree
                    get_context()->tree()->reload_children(get_node_index());
                }
            }
        });
        return true;
    }
};

REGISTER_CANVAS_TEMPLATE_NODE_EDITOR(TimelineArea, AnimatedEditor, Interpolate);

} // namespace rainynite::studio
