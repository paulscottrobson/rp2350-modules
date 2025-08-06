cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(psram)

pico_sdk_init()

option(USE_DEBUG "Build with debug support" ON) 
if (USE_DEBUG)
 add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(${MODULEDIR}/common/include)

file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")
file(GLOB_RECURSE C_SOURCES "library/*.[csS]")
file(GLOB_RECURSE COMMON_MODULE_SOURCES "${MODULEDIR}/common/library/*.[csS]")

add_executable(psram
	${APP_SOURCES} ${C_SOURCES}
	${COMMON_MODULE_SOURCES}
)

target_link_libraries(psram PUBLIC
	pico_stdlib
	pico_multicore
	hardware_dma
	pico_sync
)

pico_add_extra_outputs(psram)
