add_executable(studio main.cpp)

target_sources(studio
    PRIVATE
        main_window.cpp
        main_window.h
        time_dock.cpp
        time_dock.h
        context_listener.h
)

target_link_libraries(studio geom_helpers)

target_link_libraries(studio
    Qt5::Widgets
    document
    filters
    renderers
    fmt
    Threads::Threads
    ${SIGCPP_LIBRARIES}
)

target_include_directories(studio
    SYSTEM BEFORE PUBLIC
        ${SIGCPP_INCLUDE_DIRS}
)
