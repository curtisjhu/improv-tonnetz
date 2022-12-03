cmake_minimum_required (VERSION 3.1)
project(ImprovTonnetz CXX)

# compiler stuff
add_executable(app src/main.cpp)

# libraries
add_subdirectory(lib/Cinder)

# Includes
target_include_directories(app PRIVATE include)

# add executable with library
target_link_libraries(app cinder)


