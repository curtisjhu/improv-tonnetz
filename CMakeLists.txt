cmake_minimum_required (VERSION 2.8)
project(ImprovTonnetz cpp)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN_DIR ${CMAKE_CURRENT_BIN_DIR})

# In gcc, this will invoke the -I command
include_directories(include)


# compiler stuff
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${LIBS} m)

