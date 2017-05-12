cmake_minimum_required(VERSION 3.1)

project(studio)

set(RAINYNITE_STUDIO_VERSION "0.2" CACHE STRING "RainyNite studio version number string" FORCE)
set(RAINYNITE_STUDIO_CODENAME "Rectangle Power" CACHE STRING "RainyNite studio version codename" FORCE)

set(CMAKE_CXX_STANDARD 14) # 17 is not supported by cmake 3.7

## Qt-related stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-DQT_NO_KEYWORDS)   ## avoid slots/signals definition conflict

find_package(Qt5Widgets REQUIRED)

find_package(geom_helpers REQUIRED)
find_package(document REQUIRED)
find_package(filters REQUIRED)

find_package(Threads)

add_subdirectory(studio)
