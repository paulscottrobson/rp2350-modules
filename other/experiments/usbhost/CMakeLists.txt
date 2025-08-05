cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(host)

pico_sdk_init()

include_directories(../include)

add_executable(host
        hid_app.c
        main.c
        msc_app.c
        cdc_app.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(host PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(host PUBLIC pico_stdlib tinyusb_host tinyusb_board)

pico_add_extra_outputs(host)

# add url via pico_set_program_url
#example_auto_set_url(host)

