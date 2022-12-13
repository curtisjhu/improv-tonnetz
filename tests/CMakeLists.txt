project(UnitTests VERSION 0.1)

find_package(gtest REQUIRED)

juce_add_console_app(UnitTests PRODUCT_NAME "Unit Test Runner")

file(GLOB TEST_SOURCES *.cpp)
target_sources(UnitTests PRIVATE ${TEST_SOURCES})

target_compile_definitions(UnitTests PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
)


target_link_libraries(UnitTests PRIVATE
	GTest::gtest_main
	main.lib
)
include(GoogleTest)
gtest_discover_tests(UnitTests)
