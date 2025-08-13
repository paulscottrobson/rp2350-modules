cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(graphics)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON) 
if (USE_DEBUG)
 add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)
include_directories(${MODULEDIR}/dvi/include)

add_subdirectory(${DVIDIR} lib)
file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")
file(GLOB_RECURSE C_SOURCES "library/*.[csS]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[csS]")
file(GLOB_RECURSE DVI_MODULE_SOURCES "${MODULEDIR}/dvi/library/*.[csS]")

add_executable(graphics
	${APP_SOURCES} ${C_SOURCES}
	${COMMON_MODULE_SOURCES} ${DVI_MODULE_SOURCES}
)
target_compile_definitions(graphics PRIVATE
    DVI_USE_SIO_TMDS_ENCODER=0
    DVI_VERTICAL_REPEAT=1
    DVI_N_TMDS_BUFFERS=3
    DVI_1BPP_BIT_REVERSE=1
)


target_link_libraries(graphics PUBLIC
	pico_stdlib
	pico_multicore
	pico_util
	hardware_dma
	pico_sync
	libdvi
)

pico_add_extra_outputs(graphics)
