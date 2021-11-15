
find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    Svg
    REQUIRED)

add_executable(test-image-formats
    image_formats/main.cpp
    images.qrc
    )
target_link_libraries(test-image-formats
    Qt5::Widgets
    )