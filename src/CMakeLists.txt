project(ImprovTonnetz)

find_package(juce REQUIRED)
find_package(glm REQUIRED)

juce_add_gui_app(
	${PROJECT_NAME}
	PRODUCT_NAME ${PROJECT_NAME}
	COMPANY_NAME "curtisjhu"
	VERSION 0.1.1
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	glm::glm
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_recommended_warning_flags
	juce::juce_gui_basics
	juce::juce_audio_utils
	juce::juce_core
	juce::juce_audio_basics
	juce::juce_events
)

file(GLOB ROOT_HEADERS *.h)
file(GLOB ROOT_SOURCES *.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${ROOT_HEADERS} ${ROOT_SOURCES})

juce_generate_juce_header(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE
		JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PROJECT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")
