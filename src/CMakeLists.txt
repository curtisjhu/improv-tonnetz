project(ImprovTonnetz)

juce_add_gui_app(
	${PROJECT_NAME}
	PRODUCT_NAME ${PROJECT_NAME}
	VERSION 0.1.1
)
juce_generate_juce_header(${PROJECT_NAME})

file(GLOB ROOT_SOURCE *.cpp)
file(GLOB ROOT_HEADER *.h)

target_sources(${PROJECT_NAME} PRIVATE ${ROOT_SOURCE} ${ROOT_HEADER})

target_compile_definitions(${PROJECT_NAME} PRIVATE
		JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PROJECT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_recommended_warning_flags
	juce::juce_gui_basics
	juce::juce_audio_utils
	juce::juce_core
)

