CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(ANTILATENCY_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)


#Set toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(ToolchainFilePath "${ANTILATENCY_CMAKE_DIR}/psyq.cmake")
	if(EXISTS ${ToolchainFilePath})
		set(CMAKE_TOOLCHAIN_FILE "${ToolchainFilePath}" CACHE PATH "toolchain file" FORCE)
		message("Toolchain file found and set to ${ToolchainFilePath}")
	else()
		message(FATAL_ERROR "Unable to resolve '${ToolchainFilePath}' for setting toolchain file.")
	endif()
endif()


list(APPEND CMAKE_MODULE_PATH "${ANTILATENCY_CMAKE_DIR}")

project(PS1 LANGUAGES C)
add_executable(Test)

set_property(TARGET Test PROPERTY MSVC_RUNTIME_LIBRARY "")
target_sources(Test
PRIVATE
    main.c
	debug.c
)

MESSAGE("c init = ${CMAKE_C_FLAGS_INIT}")
#target_link_options(Test
#PRIVATE
#	-Xo$80010000
#)

#cpe2x main.cpe