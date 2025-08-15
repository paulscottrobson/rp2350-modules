cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(screeneditor)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON) 
if (USE_DEBUG)
 add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)
include_directories(${MODULEDIR}/usb/include)
include_directories(${MODULEDIR}/dvi/include)
include_directories(${MODULEDIR}/input/include)
include_directories(${MODULEDIR}/graphics/include)

add_subdirectory(${DVIDIR} lib)
file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")
file(GLOB_RECURSE C_SOURCES "library/*.[csS]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[csS]")
file(GLOB_RECURSE USB_MODULE_SOURCES "${MODULEDIR}/usb/library/*.[csS]")
file(GLOB_RECURSE DVI_MODULE_SOURCES "${MODULEDIR}/dvi/library/*.[csS]")
file(GLOB_RECURSE INPUT_MODULE_SOURCES "${MODULEDIR}/input/library/*.[csS]")
file(GLOB_RECURSE GRAPHICS_MODULE_SOURCES "${MODULEDIR}/graphics/library/*.[csS]")

add_executable(screeneditor
	${APP_SOURCES} ${C_SOURCES}
	${COMMON_MODULE_SOURCES} ${USB_MODULE_SOURCES} ${DVI_MODULE_SOURCES} ${INPUT_MODULE_SOURCES} ${GRAPHICS_MODULE_SOURCES}
)
target_compile_definitions(screeneditor PRIVATE
    DVI_USE_SIO_TMDS_ENCODER=0
    DVI_VERTICAL_REPEAT=1
    DVI_N_TMDS_BUFFERS=3
    DVI_1BPP_BIT_REVERSE=1
)


target_link_libraries(screeneditor PUBLIC
	pico_stdlib
	pico_multicore
	pico_util
	hardware_dma
	pico_sync
	libdvi
	tinyusb_host
	tinyusb_board
)

pico_add_extra_outputs(screeneditor)
