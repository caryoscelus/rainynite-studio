/*  import.h - "import" actions base
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

#ifndef STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A
#define STUDIO_ACTIONS_IMPORT_H_258614B5_435C_5FD5_B0FF_AC3B8137DB0A

#include <QFileDialog>

#include <core/node/abstract_list.h>
#include <core/node/abstract_node.h>
#include <core/util/nullptr.h>

#include <generic/action.h>
#include <util/strings.h>

namespace rainynite::studio::actions {

template <typename Processor>
class FindTargetNode {
public:
    unique_ptr<Processor> find_appropriate_target(core::NodeTree const& tree, shared_ptr<EditorContext> ctx) {
        if (auto node = ctx->get_active_node_index()) {
            if (auto r = check_target(node, ctx))
                return r;
            for (size_t i = 0; i < tree.children_count(node); ++i) {
                if (auto r = check_target(tree.index(node, i), ctx))
                    return r;
            }
        }
        auto doc_root = tree.get_root_index();
        for (size_t i = 0; i < tree.children_count(doc_root); ++i) {
            if (auto r = check_target(tree.index(doc_root, i), ctx))
                return r;
        }

        core::NodeTree::Index root;
        try {
            root = index_of_property(tree, doc_root, "root");
        } catch (...) {
            return nullptr;
        }
        for (size_t i = 0; i < tree.children_count(root); ++i) {
            if (auto r = check_target(tree.index(root, i), ctx))
                return r;
        }
        return {};
    }

    unique_ptr<Processor> check_target(core::NodeTree::Index node, shared_ptr<EditorContext> ctx) const {
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
            if (auto target = this->find_appropriate_target(*no_null(ctx->tree()), ctx)) {
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
