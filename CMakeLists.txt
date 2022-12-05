cmake_minimum_required (VERSION 3.1)
project(ImprovTonnetz CXX)

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/Cinder" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

ci_make_app(
    CINDER_PATH ${CINDER_PATH}
    SOURCES     ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    INCLUDES    ${CMAKE_CURRENT_SOURCE_DIR}/include
)