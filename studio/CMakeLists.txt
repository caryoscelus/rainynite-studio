add_executable(studio main.cpp)

target_sources(studio
    PRIVATE
        main_window.cpp
        main_window.h
)

target_link_libraries(studio
    Qt5::Widgets
    ${SIGCPP_LIBRARIES}
)

target_include_directories(studio
    PUBLIC SYSTEM BEFORE
        ${SIGCPP_INCLUDE_DIRS}
)
