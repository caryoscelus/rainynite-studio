cmake_minimum_required(VERSION 3.8)

project(rainynite-studio)

set(RAINYNITE_STUDIO_VERSION "0.7" CACHE STRING "RainyNite studio version number string" FORCE)
set(RAINYNITE_STUDIO_CODENAME "Hello World" CACHE STRING "RainyNite studio version codename" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Qt-related stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-DQT_NO_KEYWORDS)   ## avoid slots/signals definition conflict

find_package(Qt5 COMPONENTS Core Widgets Multimedia MultimediaWidgets REQUIRED)
find_package(KF5ItemModels REQUIRED)

find_package(geom_helpers REQUIRED)
find_package(rainynite_core REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(Threads)

include_directories(thirdparty/Qt-Color-Widgets/include)

add_subdirectory(thirdparty/Qt-Color-Widgets)
add_subdirectory(studio)
