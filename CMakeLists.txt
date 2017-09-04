cmake_minimum_required(VERSION 2.8.4)
project(monitor_barrera)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${CMAKE_HOME_DIRECTORY}/firmware/projects/monitor_barrera/inc")
include_directories("${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/freertos/inc")
include_directories("${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/base/inc")
include_directories("${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/board/inc")
include_directories("${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/chip/inc")
include_directories("${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/sapi_rtos/inc")

add_definitions(-DTARGET=lpc4337_m4)
add_definitions(-DBOARD=ciaa_nxp)
add_definitions(-DPROJECT=projects/monitor_barrera)

#ciaa_nxp defines

add_definitions(-DDEBUG -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -D__CODE_RED -DLPC43_MULTICORE_M0APP -D__MULTICORE_MASTER -D__MULTICORE_MASTER_SLAVE_M0APP)

#end ciaa_nxp defines

FILE(GLOB_RECURSE SRC_LIST "${CMAKE_HOME_DIRECTORY}/firmware/projects/monitor_barrera/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/projects/monitor_barrera/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/freertos/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/freertos/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/base/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/base/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/board/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/board/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/chip/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/chip/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/sapi_rtos/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/sapi_rtos/inc/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/ciaa/src/*.*"
        "${CMAKE_HOME_DIRECTORY}/firmware/modules/lpc4337_m4/ciaa/inc/*.*")

add_executable(monitor_barrera ${SRC_LIST})

add_custom_target(monitor_barrera_make WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/firmware COMMAND make)
add_custom_target(monitor_barrera_clean WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/firmware COMMAND make clean)
