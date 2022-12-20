cmake_minimum_required (VERSION 3.2)
project(ImprovTonnetz)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/CPM.cmake)

add_subdirectory(src)

option(BUILD_UNIT_TESTS "Build JUCE prototype examples" OFF)
