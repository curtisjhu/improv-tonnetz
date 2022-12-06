cmake_minimum_required (VERSION 3.1)
project(ImprovTonnetz)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



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
)



# GoogleTest
enable_testing()

add_executable(
    tests
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.cpp"
)
target_link_libraries(
    tests
    GTest::gtest_main
)
target_link_libraries(
  tests
  cinder
)

include(GoogleTest)
gtest_discover_tests(tests)