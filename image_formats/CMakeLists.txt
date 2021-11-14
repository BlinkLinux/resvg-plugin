
set(SOURCES
    src/svg_io_handler.cpp
    src/svg_io_handler.h
    src/svg_plugin.cpp
    src/svg_plugin.h
    )
qt_wrap_cpp(SOURCES
    DEPENDS svg.json)

add_library(resvg-image-formats SHARED
    ${SOURCES}
    )
message(STATUS "SOURCES: ${SOURCES}")

target_link_libraries(resvg-image-formats
    Qt5::Core
    Qt5::Gui

    resvg-lib
    )