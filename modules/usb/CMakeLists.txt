cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(filetest)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON)

if (USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)

file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")
file(GLOB_RECURSE USB_C_SOURCES "library/*.[csS]")
file(GLOB_RECURSE COMMON_MODULE "${MODULEDIR}common/library/*.[csS]")

add_executable(filetest
        ${APP_SOURCES} ${USB_C_SOURCES} 
        ${COMMON_MODULE}
)

target_link_libraries(filetest PUBLIC 
        pico_stdlib
        pico_multicore
        hardware_dma
        pico_sync
        tinyusb_host 
        tinyusb_board
)

pico_add_extra_outputs(filetest)

