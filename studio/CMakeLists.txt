add_executable(rainynite-studio main/main.cpp)

target_sources(rainynite-studio
    PRIVATE
        main/main_window.cpp
        main/main_window.h
        main/about.cpp
        main/about.h

        docks/playback_dock.cpp
        docks/playback_dock.h
        docks/time_dock.cpp
        docks/time_dock.h
        docks/node_tree_dock.cpp
        docks/node_tree_dock.h
        docks/node_edit_dock.cpp
        docks/node_edit_dock.h

        widgets/canvas.cpp
        widgets/canvas.h
        widgets/timeline_view.cpp
        widgets/timeline_view.h
        widgets/color_button.cpp
        widgets/color_button.h
        widgets/point_editor.cpp
        widgets/point_editor.h

        widgets/editor_factory.cpp

        generic/node_editor.cpp
        generic/node_editor.h
        generic/custom_widgets.cpp
        generic/custom_widgets.h
        generic/context_listener.cpp
        generic/context_listener.h
        generic/editor_context.cpp
        generic/editor_context.h
        generic/canvas_editor.cpp
        generic/canvas_editor.h

        models/node_model.cpp
        models/node_model.h

        resources.qrc
)

target_link_libraries(rainynite-studio geom_helpers)

target_link_libraries(rainynite-studio
    Qt5::Widgets
    document
    filters
    renderers
    fmt
    Threads::Threads
    ${SIGCPP_LIBRARIES}
)

target_include_directories(rainynite-studio
    SYSTEM BEFORE PUBLIC
        ${SIGCPP_INCLUDE_DIRS}
)

configure_file(version.h.cmake.in version.h)
