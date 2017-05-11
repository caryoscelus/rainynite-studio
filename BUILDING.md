RainyNite studio
================

Build Instructions
------------------

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
        mkdir -p build
        cd build
        cmake ..
        # XXX Currently no install target for tools
        make # && sudo make install
        popd
    ```

After that last step the rainynite-studio executable file should be available in `rainynite-studio/build/studio/rainynite-studio`
