cmake_minimum_required (VERSION 3.1)
project(ImprovTonnetz)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)

# Target
add_executable(
  improv
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tonnetz.cpp
)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# JUCE
FetchContent_Declare(
  jucer
  URL https://github.com/juce-framework/JUCE/releases/download/7.0.3/juce-7.0.3-osx.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(JUCER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/_deps/glm-src/glm)
FetchContent_MakeAvailable(jucer)


# GLM
FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(GLM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/_deps/jucer-src/modules)
FetchContent_MakeAvailable(glm)


# Includes
target_include_directories (
  improv
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link Libraries
target_link_libraries(
  improv
  PUBLIC glm
  PUBLIC jucer
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

include(GoogleTest)
gtest_discover_tests(tests)