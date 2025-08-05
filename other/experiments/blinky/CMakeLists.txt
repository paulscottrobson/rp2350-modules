cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(blinky)

pico_sdk_init()

include_directories(../include)

add_executable(blinky
        blinky.c
        )

target_link_libraries(blinky pico_stdlib)

pico_add_extra_outputs(blinky)

