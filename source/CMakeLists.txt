#
# CommonLibSSE-ng has to be loaded early for its configure functions and automatic linking
#
find_package(CommonLibSSE CONFIG REQUIRED)

#
# Set up the source files and output library
#
set(CURRENT_PROJECT skse_output_dll)
set(CURRENT_PROJECT_FRIENDLY_NAME "BackportedESLSupport")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(
	GLOB HEADER_FILES
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	"${SOURCE_DIR}/Impl/*.h"
	"${SOURCE_DIR}/RE/*.h"
	"${SOURCE_DIR}/*.h"
)

file(
	GLOB SOURCE_FILES
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	"${SOURCE_DIR}/Impl/*.cpp"
	"${SOURCE_DIR}/RE/*.cpp"
	"${SOURCE_DIR}/*.cpp"
)

source_group(
	TREE "${SOURCE_DIR}/.."
	FILES
		${HEADER_FILES}
		${SOURCE_FILES}
)

add_commonlibsse_plugin(
	${CURRENT_PROJECT}
	NAME ${CURRENT_PROJECT_FRIENDLY_NAME}
	AUTHOR "Nukem"
	USE_ADDRESS_LIBRARY
	SOURCES
		${HEADER_FILES}
		${SOURCE_FILES}
)

add_library("${CURRENT_PROJECT}::${CURRENT_PROJECT}" ALIAS "${CURRENT_PROJECT}")

target_precompile_headers(
	${CURRENT_PROJECT}
    PRIVATE
        PCH.h
)

target_include_directories(
	${CURRENT_PROJECT}
	PRIVATE
		"${SOURCE_DIR}"
)

set_target_properties(
	${CURRENT_PROJECT}
	PROPERTIES
		OUTPUT_NAME ${CURRENT_PROJECT_FRIENDLY_NAME}
		MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase"
)

#
# Compiler-specific options
#
target_compile_features(
	${CURRENT_PROJECT}
	PRIVATE
		cxx_std_23
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(
		${CURRENT_PROJECT}
		PRIVATE
			"/utf-8"
			"/sdl"
			"/permissive-"
			"/Zc:preprocessor"
			"/Zc:inline"
			"/EHsc"

			"/W4"
			"/wd4100"	# '': unreferenced formal parameter
			"/wd4200"	# nonstandard extension used: zero-sized array in struct/union
			"/wd4201"	# nonstandard extension used: nameless struct/union
			"/wd4324"	# '': structure was padded due to alignment specifier
	)

	target_link_options(
		${CURRENT_PROJECT}
		PRIVATE
			# Force generate release PDBs: https://learn.microsoft.com/en-us/cpp/build/reference/debug-generate-debug-info
			"$<$<CONFIG:RELEASE>:/DEBUG:FULL;/OPT:REF;/OPT:ICF>"
			# Anonymize RSDS PDB paths
			"$<$<CONFIG:RELEASE>:/PDBALTPATH:$<TARGET_FILE_PREFIX:${CURRENT_PROJECT}>$<TARGET_FILE_BASE_NAME:${CURRENT_PROJECT}>.pdb>"
	)
endif()

target_compile_definitions(
	${CURRENT_PROJECT}
	PRIVATE
		NOMINMAX
		VC_EXTRALEAN
		WIN32_LEAN_AND_MEAN
)

#
# Dependencies
#
# Spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${CURRENT_PROJECT} PRIVATE spdlog::spdlog)

# Xbyak
find_package(xbyak CONFIG REQUIRED)

#
# Commands
#
if(NOT $ENV{GAME_ROOT_DIRECTORY} STREQUAL "")
	add_custom_command(
		TARGET ${CURRENT_PROJECT}
		POST_BUILD
		COMMAND
			"${CMAKE_COMMAND}" -E copy_if_different
			"$<TARGET_FILE:${CURRENT_PROJECT}>"
			"$ENV{GAME_ROOT_DIRECTORY}/Data/SKSE/Plugins/$<TARGET_FILE_NAME:${CURRENT_PROJECT}>"
	)
endif()
