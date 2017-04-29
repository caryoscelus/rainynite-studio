RainyNite studio
================

RainyNite is a free/libre software vector animation tool, currently in early
development.

It is not really usable by end users at the moment, but developers are highly
welcome.

Build instructions
==================

These instructions assume unix-like environment.

(currently all instructions for all modules are described here; eventually,
it will be split between repos and there will be one central repo with
build script(s))

The build process itself is usual CMake procedure, but there are quite a few dependencies and code is split across repositories.

Modules:

- [Vector morphing lib](https://notabug.org/caryoscelus/rainy-morph)
- [core library](https://notabug.org/caryoscelus/rainynite-core)
- [command line tool](https://notabug.org/caryoscelus/rainynite-tool)
- [gui editor](https://notabug.org/caryoscelus/rainynite-studio)

Dependencies
------------

Core:

- [lib2geom](https://github.com/caryoscelus/lib2geom) (yes, you need my fork at the moment; also you need to build dynamic library)
- [boost](http://www.boost.org/)
- [fmt](https://github.com/fmtlib/fmt)
- [catch](https://github.com/philsquared/Catch) (bundled in repository)
- [rapidjson](https://github.com/miloyip/rapidjson) (bundled as git module)

Editor:

- [qt](https://www.qt.io/)

Build process
-------------

- rainy-morph (nothing special: just cmake build)
- core library (`git submodule update --init`, then normal build)
- tool (regular cmake build)
- studio (regular cmake build)
