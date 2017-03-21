add_library(main_window)

target_sources(main_window
    PRIVATE
        main_window.cpp
        main_window.h
)

target_link_libraries(main_window geom_helpers)

target_link_libraries(main_window
    Qt5::Widgets
    document
    filters
    renderers
    fmt
    Threads::Threads
    ${SIGCPP_LIBRARIES}
)

target_include_directories(main_window
    SYSTEM BEFORE PUBLIC
        ${SIGCPP_INCLUDE_DIRS}
)

add_executable(studio main.cpp)
target_link_libraries(studio main_window)
