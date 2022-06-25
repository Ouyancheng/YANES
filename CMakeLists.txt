cmake_minimum_required(VERSION 3.10)
project(Yanes)
include_guard(GLOBAL)

find_package(SDL2 REQUIRED)
enable_language(C ASM CXX)

set(TARGET Yanes)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)
# message(STATUS "INCLUDE_DIRS=${PROJECT_INCLUDE_DIRS}")
set(PROJECT_C_FILES
    src/cpu.c
    src/cpu_exec.c
    src/main.c
    src/ppu.c
    src/ppu_render.c
    src/sdk.c
    src/system.c
    src/mappers/mapper0.c
    src/dump_cpu.c
    # src/tests/cpu_opcode_nametable.c
    # src/tests/cpu_opcodes.c
    # src/tests/dump_cpu.c
    # src/tests/testsnakegame.c
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
add_executable(${TARGET} ${PROJECT_C_FILES} )
target_include_directories(${TARGET} PUBLIC ${PROJECT_INCLUDE_DIRS})

target_link_libraries(${TARGET} ${SDL2_LIBRARIES})


if (USE_LTO)
    include(CheckIPOSupported)
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

