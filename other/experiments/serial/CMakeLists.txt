cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(serial)

pico_sdk_init()

include_directories(../include)

add_executable(serial
        serial.c
        )

target_link_libraries(serial pico_stdlib)

pico_add_extra_outputs(serial)

