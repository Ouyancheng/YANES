cmake_minimum_required(VERION 3.10)
project(yanes)
include_guard(GLOBAL)

find_package(SDL2 REQUIRED)
enable_language(C ASM CXX)

set(TARGET yanes)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_INCLUDE_DIRS
    src
    ${SDL2_INCLUDE_DIRS}
)
set(PROJECT_C_FILES
    src/cpu_opcodes.c
    src/cpu.c
)
add_executable(${TARGET} ${PROJECT_C_FILES} )
target_include_directories(${TARGET} PUBLIC PROJECT_INCLUDE_DIRS)

target_link_libraries(${TARGET} ${SDL2_LIBRARIES})

include(CheckIPOSupported)

if (USE_LTO)
    check_ipo_supported(RESULT ipo_result OUTPUT ipo_output)
    if(ipo_result)
        set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO is not supported: ${ipo_output}")
    endif()
endif()

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -C -D "${TARGET}"  > "${TARGET}.list"
    COMMENT "Invoking: objdump"
)

