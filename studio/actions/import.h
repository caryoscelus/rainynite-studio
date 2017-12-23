/*  import.cpp - "import" actions base
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

#ifndef STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A
#define STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A

#include <QFileDialog>

#include <core/document.h>

#include <generic/action.h>
#include <util/strings.h>

namespace rainynite::studio::actions {

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

struct ImportError : public std::runtime_error {
    ImportError(string const& msg) :
        std::runtime_error(msg)
    {}
};

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
                target->start_list();
                for (auto const& s : strings)
                    target->feed(util::str(s));
                target->end_list();
            } else {
                throw ImportError("Cannot find suitable target node.");
            }
        } else {
            throw ImportError("No context! This is a bug.");
        }
    }
};

} // namespace rainynite::studio::actions

#endif
