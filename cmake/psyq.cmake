if(NOT DEFINED TOOLCHAIN_DIR)
    set(TOOLCHAIN_DIR "C:/Psyq")
endif()

#GET_FILENAME_COMPONENT(STM32_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
#list(APPEND CMAKE_MODULE_PATH ${STM32_CMAKE_DIR})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ps1)
set(CMAKE_SYSTEM_VERSION 1)


set(TOOLCHAIN_TRIPLET "mips3000")
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_TRIPLET}-")
set(TOOLCHAIN_BIN_PATH "${TOOLCHAIN_DIR}/bin")

set(CMAKE_DEPENDS_USE_COMPILER FALSE)

#set(CMAKE_C_FLAGS_INIT "")
#set(CMAKE_C_FLAGS_RELEASE  "" )
#set(CMAKE_C_FLAGS_DEBUG  "" )
#set(CMAKE_C_FLAGS  "" )

set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/CCPSX.EXE)
#set(CMAKE_C_COMPILER_FORCED TRUE)

set(CMAKE_EXECUTABLE_SUFFIX_C   .cpe)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .cpe)


include_directories(
    ${TOOLCHAIN_DIR}/include
)

#find_program(CMAKE_ASM_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_CPPFILT NAMES ${TOOLCHAIN_PREFIX}c++filt HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_DEBUGGER NAMES ${TOOLCHAIN_PREFIX}gdb HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_OBJCOPY NAMES ${TOOLCHAIN_PREFIX}objcopy HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_OBJDUMP NAMES ${TOOLCHAIN_PREFIX}objdump HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_SIZE NAMES ${TOOLCHAIN_PREFIX}size HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_AS NAMES ${TOOLCHAIN_PREFIX}as HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_AR NAMES ${TOOLCHAIN_PREFIX}ar HINTS ${TOOLCHAIN_BIN_PATH})


#set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#function(gcc_print_target_size TargetName)
#    add_custom_command(
#        TARGET ${TargetName}
#        POST_BUILD
#	    COMMAND ${CMAKE_COMMAND} -E echo "Invoking: GCC Print Size"
#        COMMAND ${CMAKE_SIZE} ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C}
#    )
#endfunction()
#
#function(gcc_generate_bin_file TargetName)
#    add_custom_command(
#        TARGET ${TargetName}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating flash image ${TargetName}.bin"
#        COMMAND ${CMAKE_OBJCOPY} -O binary ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C} ${TargetName}.bin
#        BYPRODUCTS ${TargetName}.bin
#    )
#endfunction()
#
#
#function(gcc_generate_hex_file TargetName)
#    add_custom_command(
#        TARGET ${TargetName}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating flash image ${TargetName}.hex"
#        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C} ${TargetName}.hex
#        BYPRODUCTS ${TargetName}.hex
#    )
#endfunction()
#
#
#function(gcc_generate_srec_file TargetName)
#    add_custom_command(
#        TARGET ${TargetName}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating flash image ${TargetName}.srec"
#        COMMAND ${CMAKE_OBJCOPY} -O srec ${TargetName}${CMAKE_EXECUTABLE_SUFFIX_C} ${TargetName}.srec
#        BYPRODUCTS ${TargetName}.srec
#    )
#endfunction()
#
#if(NOT (TARGET GCC::Specs::NoSys))
#    add_library(GCC::Specs::NoSys INTERFACE IMPORTED)
#    target_link_options(GCC::Specs::NoSys INTERFACE 
#        -specs=nosys.specs
#    )
#endif()
#
#if(NOT (TARGET GCC::Specs::Nano))
#    add_library(GCC::Specs::Nano INTERFACE IMPORTED)
#    target_link_options(GCC::Specs::Nano INTERFACE 
#        -specs=nano.specs
#    )
#endif()
#
#if(NOT (TARGET GCC::ReleaseLto))
#    add_library(GCC::ReleaseLto INTERFACE IMPORTED)
#    target_link_options(GCC::ReleaseLto INTERFACE $<$<CONFIG:RELEASE>:-flto>)
#    target_compile_options(GCC::ReleaseLto INTERFACE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>:-flto>)
#    target_compile_options(GCC::ReleaseLto INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:-flto>)
#endif()
#
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
#
#
#function(gcc_add_linker_script TARGET VISIBILITY SCRIPT_PATH)
#    get_filename_component(SCRIPT_PATH "${SCRIPT_PATH}" ABSOLUTE)
#    target_link_options(${TARGET} ${VISIBILITY} -T "${SCRIPT_PATH}")
#endfunction()


