cmake_minimum_required(VERSION 3.12)
project(resvg_plugin VERSION 0.1.1)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Default build type: Release" FORCE)
endif ()

option(BUILD_TESTING "Build test targets" OFF)

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

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
