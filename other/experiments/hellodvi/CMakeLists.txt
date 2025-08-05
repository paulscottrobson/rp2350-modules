cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(hellodvi)

pico_sdk_init()

include_directories(../include)

add_subdirectory(libdvi)


add_executable(hellodvi
        main.c
        )

target_compile_definitions(hellodvi PRIVATE
    DVI_USE_SIO_TMDS_ENCODER=0
#    DVI_VERTICAL_REPEAT=1
#    DVI_N_TMDS_BUFFERS=3
#    DVI_1BPP_BIT_REVERSE=1
)

target_link_libraries(hellodvi 
    pico_stdlib
    pico_multicore
    pico_util
    libdvi
)

pico_add_extra_outputs(hellodvi)

