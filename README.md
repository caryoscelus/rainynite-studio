RainyNite studio
================

RainyNite is a free/libre software vector animation tool, currently in early
development.

It is not really usable by end users at the moment, but developers are highly
welcome.

Usage examples
==============

- couple of animations in [an ld38 entry][ld38] (namely sunrise around 10s of
  [intro][ld38_intro] and animated logo [here][ld38_logo])
- see <https://notabug.org/caryoscelus/rainynite-examples> for sample files

Screenshots
===========

![sunrise editing screenshot](https://caryoscelus.github.io/images/screenshots/2017.04.30-sunrise.png)

[ld38]: https://ldjam.com/events/ludum-dare/38/power-department
[ld38_intro]: https://media.githubusercontent.com/media/spirulence/power-department-ld38/e4e0915cd6fe2f2f2bc9510c637ffaba8c55ee4d/assets/video/intro-720p.webm
[ld38_logo]: https://media.githubusercontent.com/media/spirulence/power-department-ld38/e4e0915cd6fe2f2f2bc9510c637ffaba8c55ee4d/assets/video/03_warning.webm

Build instructions
==================

These instructions assume unix-like environment.

(currently all instructions for all modules are described here; eventually,
it will be split between repos and there will be one central repo with
build script(s))

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

Editor:

- [qt](https://www.qt.io/)

Runtime renderer dependency:

- [Inkscape](https://inkscape.org/) - used to render SVG output of RainyNite "renderer"; can be replaced with other command line SVG renderer

Build process
-------------

Note: for your convenience, you can download a build script for Debian and derivatives from https://gist.github.com/s-leroux/d0ca50b1a0a772c3db631070281b24f4

Here are the instructions to build RainyNite-Studio from scratch:

1. Install the core developer tools `git`, `make`, `cmake` and `gcc`/`g++`
2. Download the developer packages for the dependencies given above.
On Debian and derivatives, you have to install:

    * `libblas-dev`
    * `libgsl-dev`
    * `python-cairo-dev`
    * `libgtk2.0-dev`
    * `libgtkmm-2.4-dev`
    * `libcairomm-1.0-dev`
    * `libcairo2-dev`
    * `libboost-dev`
    * `qtbase5-dev`.
    
    **You do not need to install `fmt` or `lib2geom` since we will build them from sources.**
3. Download, compile & install `lib2geom`:

    ```bash
        git clone https://github.com/caryoscelus/lib2geom
        pushd lib2geom
        git submodule init
        git submodule update
        mkdir -p build
        cd build
        cmake -D2GEOM_BUILD_SHARED=ON \
                -D2GEOM_BOOST_PYTHON=OFF -D2GEOM_CYTHON_BINDINGS=OFF -D2GEOM_TOYS=OFF \
                ..
        make && sudo make install
        popd
    ```

4. Download, compile & install `fmt`:

    ```bash
        git clone https://github.com/fmtlib/fmt
        pushd fmt
        mkdir -p build
        cd build
        cmake -DBUILD_SHARED_LIBS=ON \
                ..
        make && sudo make install
        popd
    ```

5. Download, compile & install the rainy-morph and rainynite-core modules:

    ```bash
        # rainy-morph
        git clone https://notabug.org/caryoscelus/rainy-morph
        pushd rainy-morph
        mkdir -p build
        cd build
        cmake ..
        make && sudo make install
        popd
        
        # rainy-core
        git clone https://notabug.org/caryoscelus/rainynite-core
        pushd rainynite-core
        git submodule update --init
        mkdir -p build
        cd build
        cmake ..
        make && sudo make install
        popd
    ```

6. Download, compile & install the rainynite-studio

    ```bash
        git clone https://notabug.org/caryoscelus/rainynite-studio
        pushd rainynite-studio
        cd build
        mkdir -p build
        cmake ..
        # XXX Currently no install target for tools
        make # && sudo make install
        popd
    ```

After that last step the rainynite-studio executable file should be available in `rainynite-studio/build/studio/rainynite-studio`
