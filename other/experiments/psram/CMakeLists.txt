cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(psram)

pico_sdk_init()

include_directories(../include)

add_executable(psram
        psram.c
        rp2_psram.c
        )

target_link_libraries(psram pico_stdlib)

pico_add_extra_outputs(psram)

