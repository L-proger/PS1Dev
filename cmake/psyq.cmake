if(NOT DEFINED TOOLCHAIN_DIR)
    set(TOOLCHAIN_DIR "$ENV{Ps1Sdk}/compiler/mipsel-none-elf-gcc-12.2.0-binutils-2.39")
endif()


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ps1)
set(CMAKE_SYSTEM_VERSION 1)

set(TOOLCHAIN_TRIPLET "mipsel-none-elf")
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_TRIPLET}-")
set(TOOLCHAIN_BIN_PATH "${TOOLCHAIN_DIR}/bin")


find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}g++ HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_ASM_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_CPPFILT NAMES ${TOOLCHAIN_PREFIX}c++filt HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_DEBUGGER NAMES ${TOOLCHAIN_PREFIX}gdb HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_OBJCOPY NAMES ${TOOLCHAIN_PREFIX}objcopy HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_OBJDUMP NAMES ${TOOLCHAIN_PREFIX}objdump HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_SIZE NAMES ${TOOLCHAIN_PREFIX}size HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_AS NAMES ${TOOLCHAIN_PREFIX}as HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_AR NAMES ${TOOLCHAIN_PREFIX}ar HINTS ${TOOLCHAIN_BIN_PATH})


function(gcc_add_linker_script TARGET VISIBILITY SCRIPT_PATH)
    get_filename_component(SCRIPT_PATH "${SCRIPT_PATH}" ABSOLUTE)
    target_link_options(${TARGET} ${VISIBILITY} -T "${SCRIPT_PATH}")
endfunction()


set(CMAKE_EXECUTABLE_SUFFIX_C   .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#find_program(CPE2X_EXE NAMES CPE2X HINTS ${TOOLCHAIN_BIN_PATH})

#function(cpe2x TargetName)
#    add_custom_command(
#        TARGET ${TargetName}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating PlayStation1 EXE file"
#        COMMAND ${CPE2X_EXE} ${TargetName}.cpe
#        BYPRODUCTS ${TargetName}.exe
#    )
#endfunction()


function(gcc_print_target_size TargetName)
    add_custom_command(
        TARGET ${TargetName}
        POST_BUILD
	    COMMAND ${CMAKE_COMMAND} -E echo "Invoking: GCC Print Size"
        COMMAND ${CMAKE_SIZE} ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C}
    )
endfunction()

function(gcc_generate_bin_file TargetName)
    add_custom_command(
        TARGET ${TargetName}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generating flash image ${TargetName}.bin"
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C} ${TargetName}.bin
        BYPRODUCTS ${TargetName}.bin
    )
endfunction()


function(gcc_generate_hex_file TargetName)
    add_custom_command(
        TARGET ${TargetName}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generating flash image ${TargetName}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C} ${TargetName}.hex
        BYPRODUCTS ${TargetName}.hex
    )
endfunction()



#if(NOT (TARGET GCC::AssemblerWithCpp))
#    add_library(GCC::AssemblerWithCpp INTERFACE IMPORTED)
#    target_compile_options(GCC::AssemblerWithCpp INTERFACE $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>)
#endif()
#
#
#if(NOT (TARGET GCC::DefaultOptimizationFlags))
#    add_library(GCC::DefaultOptimizationFlags INTERFACE IMPORTED)
#    
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>:-O0 -g>)
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>:-O0 -g>)
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:DEBUG>>:-g>)
#
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>:-O3>)
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:-O3>)
#
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:MINSIZEREL>>:-Os>)
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:MINSIZEREL>>:-Os>)
#
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELWITHDEBINFO>>:-O3 -g>)
#    target_compile_options(GCC::DefaultOptimizationFlags INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELWITHDEBINFO>>:-O3 -g>)
#endif()
