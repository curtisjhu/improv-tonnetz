
# temp

# Cinder
FetchContent_Declare(
  cinder
  URL https://github.com/cinder/Cinder/archive/bfd055817bc6f04b4ff6a410932618c7eef0d78c.zip
)
FetchContent_MakeAvailable(cinder)
get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/_deps/cinder-src/" ABSOLUTE)
include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
ci_make_app(
    CINDER_PATH ${CINDER_PATH}
    SOURCES     ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/main.h
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.h
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.cpp
    INCLUDES    ${CMAKE_CURRENT_SOURCE_DIR}/include
                ${CINDER_PATH}/include