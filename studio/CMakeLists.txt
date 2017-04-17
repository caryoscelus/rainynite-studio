add_executable(studio main.cpp)

target_sources(studio
    PRIVATE
        main_window.cpp
        main_window.h

        playback_dock.cpp
        playback_dock.h
        time_dock.cpp
        time_dock.h
        node_tree_dock.cpp
        node_tree_dock.h
        node_edit_dock.cpp
        node_edit_dock.h

        canvas.cpp
        canvas.h
        timeline_view.cpp
        timeline_view.h

        node_editor.cpp
        node_editor.h
        custom_widgets.cpp
        custom_widgets.h
        context_listener.cpp
        context_listener.h

        node_model.cpp
        node_model.h

        color_button.cpp
        color_button.h

        color_widget.cpp

        resources.qrc
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
