cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(artdvi)

pico_sdk_init()

include_directories(include)
include_directories(include.here)
add_subdirectory(../../../picodvi/software/libdvi lib)

add_executable(artdvi
        main.here/main.c
        dvi_driver.c
        tmds_encode_custom.S
        )

target_compile_definitions(artdvi PRIVATE
    DVI_USE_SIO_TMDS_ENCODER=0
    DVI_VERTICAL_REPEAT=1
    DVI_N_TMDS_BUFFERS=3
    DVI_1BPP_BIT_REVERSE=1
)

target_link_libraries(artdvi 
    pico_stdlib
    pico_multicore
    pico_util
    libdvi
)

pico_add_extra_outputs(artdvi)

