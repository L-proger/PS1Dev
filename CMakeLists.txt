CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

#Set toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(ToolchainFilePath "${CMAKE_DIR}/psyq.cmake")
	if(EXISTS ${ToolchainFilePath})
		set(CMAKE_TOOLCHAIN_FILE "${ToolchainFilePath}" CACHE PATH "toolchain file" FORCE)
		message("Toolchain file found and set to ${ToolchainFilePath}")
	else()
		message(FATAL_ERROR "Unable to resolve '${ToolchainFilePath}' for setting toolchain file.")
	endif()
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_DIR}")

project(PS1 LANGUAGES C CXX ASM)

set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

add_executable(Test)

set_property(TARGET Test PROPERTY MSVC_RUNTIME_LIBRARY "")
target_sources(Test
PRIVATE
	cube.cpp
	cube.h
    main.cpp
	debug.c
	config.h
	textures/Test_64x64.s
	${CMAKE_CURRENT_LIST_DIR}/common/crt0/crt0.s
)


target_include_directories(Test
PRIVATE
	C:/Psyq/include
	${CMAKE_CURRENT_LIST_DIR}
)


set(ARCHFLAGS
	-march=mips1
	-mabi=32
	-EL
	-fno-pic
	-mno-shared
	-mno-abicalls
	-mfp32
	-fno-stack-protector
	-nostdlib
	-ffreestanding
)

target_compile_options(Test
PRIVATE
	#-v
	${ARCHFLAGS}

	-mno-gpopt
	-fomit-frame-pointer
	-fno-builtin
	-fno-strict-aliasing
	-Wno-attributes
	-nodefaultlibs
	-g
)




target_link_directories(Test
PRIVATE
	E:/L/GameConsole/PSOne/tools/MyToolchain/psyq-4.7-converted-full/lib
)

target_link_libraries(Test
PRIVATE
	GCC::Cxx::NoExceptions
	GCC::Cxx::NoRtti

	-Wl,--start-group
	api
	c
	c2
	card
	comb
	ds
	etc
	gpu
	gs
	gte
	gun
	hmd
	math
	mcrd
	mcx
	pad
	press
	sio
	snd
	spu
	tap
	-Wl,--end-group

	${ARCHFLAGS}

	-Wl,--oformat=elf32-littlemips

	-nostdlib
	-nodefaultlibs
	-static
	-Wl,--gc-sections

	-g
)

gcc_add_linker_script(Test
PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/ps1-exe.ld
)

gcc_add_linker_script(Test
PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/default.ld
)

gcc_print_target_size(Test)
elf2x(Test)
