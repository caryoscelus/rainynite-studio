RainyNite studio
================

RainyNite is a free/libre software 2d vector animation tool in development.

STATUS
======

Over the course of RN development i've come to conclusion that a large part of
it was needlessly reinvented DSL, ineffectively implemented in C++. At this
point i'm considering option of porting it to Haskell or possibly another
functional platform. In case you have an existing files that you'd like to
preserve, please contact me.

New version will be announced here as soon as it is available.

Usage examples
==============

- couple of animations in [an ld38 entry][ld38] (namely sunrise around 10s of
  [intro][ld38_intro] and animated logo [here][ld38_logo])
- see <https://notabug.org/caryoscelus/rainynite-examples> for sample files

Screenshots
===========

!["pulse of life" editing screenshot (v0.6)](https://caryoscelus.github.io/images/screenshots/2017.10.25-pulse.png)
![bouncing ball editing screenshot](https://caryoscelus.github.io/images/screenshots/2017.07.29-ball.png)
![sunrise editing screenshot](https://caryoscelus.github.io/images/screenshots/2017.04.30-sunrise.png)

[ld38]: https://ldjam.com/events/ludum-dare/38/power-department
[ld38_intro]: https://media.githubusercontent.com/media/spirulence/power-department-ld38/e4e0915cd6fe2f2f2bc9510c637ffaba8c55ee4d/assets/video/intro-720p.webm
[ld38_logo]: https://media.githubusercontent.com/media/spirulence/power-department-ld38/e4e0915cd6fe2f2f2bc9510c637ffaba8c55ee4d/assets/video/03_warning.webm

Build instructions
==================

These instructions assume unix-like environment.

The build process itself is usual CMake procedure, but there are quite a few dependencies and code is split across repositories.

Modules:

- [vector morphing lib](https://notabug.org/caryoscelus/rainy-morph)
- [core library](https://notabug.org/caryoscelus/rainynite-core)
- [command line tool](https://notabug.org/caryoscelus/rainynite-tool) (not really worth using atm)
- [gui editor](https://notabug.org/caryoscelus/rainynite-studio)

Dependencies
------------

Core:

- [lib2geom](https://github.com/caryoscelus/lib2geom) (yes, you need my fork at the moment; also you need to build & install dynamic library)
- [boost](http://www.boost.org/)
- [fmt](https://github.com/fmtlib/fmt)
- [catch](https://github.com/philsquared/Catch) (bundled in repository)
- [rapidjson](https://github.com/miloyip/rapidjson) (bundled as git module)
- [yaml-cpp](https://github.com/caryoscelus/yaml-cpp) (bundled as git module)

Editor:

- [qt](https://www.qt.io/)
- [KDE Frameworks][https://kde.org/]
- [Qt-Color-Widgets](https://github.com/mbasaglia/Qt-Color-Widgets) (bundled as git module)

Runtime renderer dependency:

- [Inkscape](https://inkscape.org/) - used to render SVG output of RainyNite "renderer"; can be replaced with other command line SVG renderer
- ffmpeg: optional, for waveform generation

Build process
-------------

The easy way is to clone [united repository](https://notabug.org/caryoscelus/rainynite)
and run `update_and_build.sh` script.

However, if you're interested in development, you may find it more convenient to
setup build directories yourself. Build process is mostly usual CMake routine,
except for a few caveats. You can look them up at [BUILDING.md][BUILDING.md] or
in [build script](https://notabug.org/caryoscelus/rainynite/src/master/update_and_build.sh).

License
-------

Code license is GPLv3+:

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Full license text is available in COPYING.gpl3.

Third-party modules may have its own license terms apply.

All original non-code content is licensed under CC-BY-SA 4.0 unless noted
otherwise.

`pulse.webm` soundtrack uses heartbeat sound by Benboncan, which is available at
<https://freesound.org/people/Benboncan/sounds/108207/>.
