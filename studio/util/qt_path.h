/*  qt_path.h - Qt <-> lib2geom path conversion
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

#ifndef STUDIO_UTIL_QT_PATH_H_3FCA40F9_C398_5905_B884_1EA23BEF81F4
#define STUDIO_UTIL_QT_PATH_H_3FCA40F9_C398_5905_B884_1EA23BEF81F4

/**
 * TODO: move to separate Qt lib2geom wrapper library, perhaps?
 * TODO: namespace
 */

#include <QPainterPath>

#include <2geom/path-sink.h>

#include <geom_helpers/knots.h>


namespace rainynite::util {

class QtPathSink : public Geom::PathSink {
public:
    void moveTo(Geom::Point const& p) override {
        target.moveTo(p.x(), p.y());
    }
    void lineTo(Geom::Point const& p) override {
        target.lineTo(p.x(), p.y());
    }
    void curveTo(Geom::Point const& c0, Geom::Point const& c1, Geom::Point const& p) override {
        target.cubicTo(
            c0.x(), c0.y(),
            c1.x(), c1.y(),
            p.x(), p.y()
        );
    }
    void quadTo(Geom::Point const& c, Geom::Point const& p) override {
        target.quadTo(
            c.x(), c.y(),
            p.x(), p.y()
        );
    }
    void arcTo(Geom::Coord /*rx*/, Geom::Coord /*ry*/, Geom::Coord /*angle*/, bool /*large_arc*/, bool /*sweep*/, Geom::Point const& /*p*/) override {
        throw std::runtime_error("QtPathSink: arcs not supported");
    }

    void closePath() override {
        target.closeSubpath();
    }
    void flush() override {
    }

    QPainterPath get() {
        return target;
    }

private:
    QPainterPath target;
};

inline QPainterPath path_to_qt(Geom::BezierKnots const& path) {
    QtPathSink sink;
    sink.feed(Geom::knots_to_path(path));
    return sink.get();
}

} // namespace rainynite::util

#endif
