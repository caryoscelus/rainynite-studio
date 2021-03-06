add_executable(rainynite-studio main/main.cpp)

target_sources(rainynite-studio
    PRIVATE
        main/main_window.cpp
        main/main_window.h
        main/about.cpp
        main/about.h
        main/renderer.cpp
        main/renderer.h
        main/audio.h
        main/audio.cpp

        docks/close_destroy_dock.h
        docks/playback.h
        docks/playback.cpp
        docks/timearea.h
        docks/timearea.cpp
        docks/node_tree.h
        docks/node_tree.cpp
        docks/node_edit.h
        docks/node_edit.cpp
        docks/advanced_color_selector.h
        docks/advanced_color_selector.cpp
        docks/undo_history.h
        docks/undo_history.cpp
        docks/log.h
        docks/log.cpp

        widgets/editor_factory.cpp

        widgets/canvas.cpp
        widgets/canvas.h
        widgets/timeline_area.cpp
        widgets/timeline_area.h
        widgets/timeline_ruler.cpp
        widgets/timeline_ruler.h
        widgets/color_button.cpp
        widgets/color_button.h
        widgets/point_value_editor.cpp
        widgets/point_value_editor.h
        widgets/boolean_editor.cpp
        widgets/multiline_editor.cpp
        widgets/bezier_editor.cpp
        widgets/file_path_editor.h
        widgets/file_path_editor.cpp

        canvas/abstract_canvas.h
        canvas/abstract_canvas.cpp
        canvas/abstract_editor.h
        canvas/editor.cpp
        canvas/tool.h
        canvas/tool.cpp
        canvas/attachable.h
        canvas/attachable.cpp

        canvas/tools/base.h
        canvas/tools/default.cpp
        canvas/tools/shape.h
        canvas/tools/shape.cpp
        canvas/tools/zoom_area.cpp
        canvas/tools/rectangle.cpp
        canvas/tools/circle.cpp
        canvas/tools/bezier.cpp
        canvas/tools/transform_move.cpp
        canvas/tools/time_default.cpp

        canvas/editors/point_item.h
        canvas/editors/point_item.cpp
        canvas/editors/point_editor.cpp
        canvas/editors/bezier_editor.cpp
        canvas/editors/bezier_editor.h
        canvas/editors/rectangle_editor.cpp

        canvas/editors/time_item.h
        canvas/editors/time_item.cpp
        canvas/editors/timeline_cursor.cpp
        canvas/editors/time_editor.cpp
        canvas/editors/time_period_editor.cpp
        canvas/editors/time_bezier.cpp
        canvas/editors/waveform.cpp
        canvas/editors/animated.cpp

        generic/node_menu.cpp
        generic/node_menu.h
        generic/node_editor.cpp
        generic/node_editor.h
        generic/custom_widgets.cpp
        generic/custom_widgets.h
        generic/context_listener.cpp
        generic/context_listener.h
        generic/editor_context.cpp
        generic/editor_context.h
        generic/canvas_editor.h
        generic/timeline_editor.h
        generic/timeline_editor.cpp
        generic/dock_registry.h
        generic/action.h
        generic/action_menu.cpp
        generic/process_node.h

        actions/import.h
        actions/external_player.cpp
        actions/import.cpp
        actions/import_soundtrack.cpp

        models/model_filter.h
        models/node_model.cpp
        models/node_model.h
        models/node_list.cpp
        models/node_list.h
        models/undo_model.h
        models/undo_model.cpp

        util/strings.h
        util/geom.h
        util/qt_path.h
        util/pen.h
        util/pen.cpp

        resources.qrc
)

target_link_libraries(rainynite-studio
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    KF5::ItemModels
    KF5::Crash
    ColorWidgets-qt5
    nodes
    filters
    svg_renderer
    fmt
    boost_system
    boost_filesystem
    Threads::Threads
)

configure_file(version.h.cmake.in version.h)

install(
    TARGETS rainynite-studio
    RUNTIME DESTINATION bin
)
