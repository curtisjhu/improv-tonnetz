
include(CPM)

option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
CPMAddPackage("gh:juce-framework/JUCE#7.0.3")