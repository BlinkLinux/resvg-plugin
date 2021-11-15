cmake_minimum_required(VERSION 3.12)
project(resvg_plugin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS
    Core
    Gui
    REQUIRED)

add_subdirectory(rust)
add_subdirectory(imageformats)
add_subdirectory(tests)