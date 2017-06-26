/*
 *  bezier_editor.cpp - edit beziers on canvas
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

#include <fmt/format.h>

#include <QGraphicsItem>
#include <QDebug>

#include <geom_helpers/knots.h>

#include <2geom/path-sink.h>

#include <widgets/canvas.h>
#include "point_item.h"
#include "bezier_editor.h"

using namespace fmt::literals;

namespace studio {

BezierKnotsDisplay::BezierKnotsDisplay() {
    init();
}

BezierKnotsDisplay::~BezierKnotsDisplay() {
    uninit();
}

void BezierKnotsDisplay::set_canvas(Canvas* canvas) {
    uninit();
    CanvasEditor::set_canvas(canvas);
    init();
}

void BezierKnotsDisplay::set_node(std::shared_ptr<core::AbstractValue> node) {
    uninit();
    NodeEditor::set_node(node);
    init();
}

void BezierKnotsDisplay::time_changed(core::Time) {
    // TODO
    uninit();
    init();
}

void move_point_to(std::shared_ptr<core::BaseValue<Geom::BezierKnots>> bezier_node, Geom::Point Geom::Knot::* point, size_t index, double x, double y) {
    auto& path = bezier_node->mod();
    (path.knots[index].*point).x() = x;
    (path.knots[index].*point).y() = y;
    bezier_node->changed();
}

// TODO: move elsewhere
class QtPathSink : public Geom::PathSink {
public:
    virtual void moveTo(Geom::Point const &p) override {
        // TODO: Geom::Point <-> QGeom::PointF convertors
        target.moveTo(p.x(), p.y());
    }
    virtual void lineTo(Geom::Point const &p) override {
        target.lineTo(p.x(), p.y());
    }
    virtual void curveTo(Geom::Point const &c0, Geom::Point const &c1, Geom::Point const &p) override {
        target.cubicTo(
            c0.x(), c0.y(),
            c1.x(), c1.y(),
            p.x(), p.y()
        );
    }
    virtual void quadTo(Geom::Point const &c, Geom::Point const &p) override {
        target.quadTo(
            c.x(), c.y(),
            p.x(), p.y()
        );
    }
    virtual void arcTo(Geom::Coord /*rx*/, Geom::Coord /*ry*/, Geom::Coord /*angle*/, bool /*large_arc*/, bool /*sweep*/, Geom::Point const &/*p*/) override {
        throw std::runtime_error("QtPathSink: arcs not supported");
    }

    virtual void closePath() override {
        target.closeSubpath();
    }
    virtual void flush() override {
    }
public:
    inline QPainterPath get() {
        return target;
    }
private:
    QPainterPath target;
};

QPainterPath path_to_qt(Geom::BezierKnots const& path) {
    QtPathSink sink;
    sink.feed(Geom::knots_to_path(path));
    return sink.get();
}

void BezierKnotsDisplay::init() {
    if (auto canvas = get_canvas()) {
        if (auto scene = canvas->scene()) {
            if (auto bezier_node = std::dynamic_pointer_cast<core::BaseValue<Geom::BezierKnots>>(get_node())) {
                bool readonly = !bezier_node->can_set();

                Geom::BezierKnots path;
                try {
                    path = bezier_node->get(get_core_context()->get_time());
                } catch (std::exception const& ex) {
                    qDebug() << QString::fromStdString("Uncaught exception while getting path: {}"_format(ex.what()));
                    return;
                }

                auto e = scene->addPath(path_to_qt(path));
                knot_items.emplace_back(e);

                auto add_point_editor = [
                    this,
                    scene,
                    readonly
                ](auto const& point, PointItem::Callback callback) {
                    auto e = new PointItem(callback);
                    e->set_pos(point.x(), point.y());
                    e->set_readonly(readonly);
//                     scene->addItem(e);
                    return e;
                };

                size_t i = 0;
                for (auto const& knot : path.knots) {
                    auto pos = add_point_editor(
                        knot.pos,
                        [this, i, bezier_node](double x, double y) {
                            move_point_to(bezier_node, &Geom::Knot::pos, i, x, y);
                        }
                    );
                    scene->addItem(pos);
                    auto tg1 = add_point_editor(
                        knot.tg1,
                        [this, i, bezier_node](double x, double y) {
                            move_point_to(bezier_node, &Geom::Knot::tg1, i, x, y);
                        }
                    );
                    tg1->setParentItem(pos);
                    auto tg2 = add_point_editor(
                        knot.tg2,
                        [this, i, bezier_node](double x, double y) {
                            move_point_to(bezier_node, &Geom::Knot::tg2, i, x, y);
                        }
                    );
                    tg2->setParentItem(pos);
                    knot_items.emplace_back(pos);
//                     qDebug() << (size_t)pos.get() << (size_t)tg1.get() << (size_t)tg2.get();

                    if (!knot.uid.empty()) {
                        auto e = scene->addText(QString::fromStdString(knot.uid));
                        e->setX(knot.pos.x());
                        e->setY(knot.pos.y());
                        knot_items.emplace_back(e);
                    }

                    ++i;
                }
            }
        }
    }
}

void BezierKnotsDisplay::uninit() {
    if (auto canvas = get_canvas()) {
        for (auto const& e : knot_items) {
            canvas->scene()->removeItem(e.get());
        }
        knot_items.clear();
    }
}

}
