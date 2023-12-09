cmake_minimum_required(VERSION 3.26)

project(
    backported-esl-support
    VERSION 1.1
    LANGUAGES CXX)

set(PROJECT_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/source")

#
# Store the current git commit hash for later use
#
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    OUTPUT_VARIABLE BUILD_GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#
# Set up the actual library
#
add_subdirectory("${PROJECT_SOURCE_PATH}")

#
# And finally produce build artifacts
#
set(PLUGIN_LOADER_OUTPUT_DIR "Data/SKSE/Plugins/")
set(CPACK_PACKAGE_FILE_NAME "BEES-${CMAKE_PROJECT_VERSION_MAJOR}_${CMAKE_PROJECT_VERSION_MINOR}")

install(
    TARGETS
        skse_output_dll
    RUNTIME
        DESTINATION ${PLUGIN_LOADER_OUTPUT_DIR}
)

set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
