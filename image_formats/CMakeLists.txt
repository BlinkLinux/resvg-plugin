
add_library(resvg-image-formats SHARED
    src/svg_plugin.cpp
    src/svg_plugin.h
    )

target_link_libraries(resvg-image-formats
    Qt5::Core
    Qt5::Gui
    )