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
include_directories(${MODULEDIR}/modes/include)
include_directories(${MODULEDIR}/usb/include)

file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")
file(GLOB_RECURSE C_SOURCES "library/*.[csS]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[csS]")
file(GLOB_RECURSE DVI_MODULE_SOURCES "${MODULEDIR}/dvi/library/*.[csS]")
file(GLOB_RECURSE USB_MODULE_SOURCES "${MODULEDIR}/usb/library/*.[csS]")
file(GLOB_RECURSE MODES_MODULE_SOURCES "${MODULEDIR}/modes/library/*.[csS]")

add_executable(graphics
	${APP_SOURCES} ${C_SOURCES} 
	${COMMON_MODULE_SOURCES} ${DVI_MODULE_SOURCES} ${MODES_MODULE_SOURCES} ${USB_MODULE_SOURCES}
)

target_link_libraries(graphics PUBLIC
	pico_stdlib
	pico_multicore
	hardware_dma
	pico_sync
    tinyusb_host
    tinyusb_board    
)

pico_add_extra_outputs(graphics)
