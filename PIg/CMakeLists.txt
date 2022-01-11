##########################################################################
# "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware
# license):
# <dev@layer128.net> wrote this file. As long as you retain this notice
# you can do whatever you want with this stuff. If we meet some day, and
# you think this stuff is worth it, you can buy me a be(ve)er(age) in
# return. (I don't like beer much.)
#
# Matthias Kleemann
##########################################################################

cmake_minimum_required(VERSION 3.0)

### TOOLCHAIN SETUP AREA #################################################
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.
##########################################################################

##########################################################################
# tools to beused for programming the AVR
##########################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avrispmkII)
set(AVR_UPLOADTOOL_PORT usb)

##########################################################################
# AVR and fuses needs to be set
##########################################################################
set(AVR_MCU atmega128a)
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)
set(AVR_FCPU "16000000UL")

### END TOOLCHAIN SETUP AREA #############################################

include(avr/generic-gcc-avr.cmake)

##########################################################################
# name your project
##########################################################################
project(PIg)
set(PROJECT_VERSION_MAJOR 0x01)
set(PROJECT_VERSION_MINOR 0x52)

##########################################################################
# status messages
##########################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

##########################################################################
# create version file
##########################################################################
configure_file(version.hpp.in ../src/version.hpp)

##########################################################################
# set build type
##########################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

##########################################################################
# some cmake cross-compile necessities
##########################################################################
set(CMAKE_CXX_STANDARD 17)
if(DEFINED ENV{AVR_FIND_ROOT_PATH})
    set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else(DEFINED ENV{AVR_FIND_ROOT_PATH})
    if(EXISTS "/opt/local/avr")
      set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
    elseif(EXISTS "/usr/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/avr")
    elseif(EXISTS "/usr/lib/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
    elseif(EXISTS "/usr/local/CrossPack-AVR")
      set(CMAKE_FIND_ROOT_PATH "/usr/local/CrossPack-AVR")
    else(EXISTS "/opt/local/avr")
      message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
    endif(EXISTS "/opt/local/avr")
endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

##########################################################################
# status messages for generating
##########################################################################
message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

##########################################################################
# set compiler options for build types
##########################################################################
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
   set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O1 -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_DEBUG "-O1 -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

##########################################################################
# compiler options for all build types
##########################################################################
add_definitions("-DF_CPU=${AVR_FCPU}")
add_definitions("-DAVR")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-Wall")
#add_definitions("-Werror")
add_definitions("-pedantic")
add_definitions("-pedantic-errors")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-ffunction-sections")
add_definitions("-c")

##########################################################################
# include search paths
##########################################################################
include_directories(
    src
)

##########################################################################
# building library and application in their subdirectories
##########################################################################
add_avr_executable( PIg
    main.cpp
    src/avr.h
    src/flash.h
    src/flashArrays.c
    src/flashArrays.h
    src/flashParams.cpp
    src/flashParams.h
    src/glbDefine.h
    src/glbTypes.h
    src/paramBsp.h
    src/paramDef.h
    src/paramGlb.h
    src/paramPrd.h
    src/paramPrm.h
    src/paramUart.h
    src/debug/debug.cpp
    src/debug/debug.hpp
    src/drivers/kbdLayout.cpp
    src/drivers/kbdLayout.h
    src/drivers/keyboard.cpp
    src/drivers/keyboard.h
    src/drivers/ks0108.cpp
    src/drivers/ks0108.h
    src/drivers/symbols.h
    src/drivers/tmp75.cpp
    src/drivers/tmp75.h
    src/drivers/uart.cpp
    src/drivers/uart.h
    src/init/init.cpp
    src/menu/base.cpp
    src/menu/base.h
    src/menu/control.cpp
    src/menu/control.h
    src/menu/dateTime.cpp
    src/menu/dateTime.h
    src/menu/enterParam.cpp
    src/menu/enterParam.h
    src/menu/measure.h
    src/menu/menu.cpp
    src/menu/menu.h
    src/menu/menuPunkt.h
    src/menu/password.h
    src/menu/test.cpp
    src/menu/test.h
    src/menu/txCom.cpp
    src/menu/txCom.h
    src/parameter/LocalParams.cpp
    src/parameter/LocalParams.h
    src/parameter/param.cpp
    src/parameter/param.h
    src/protocols/iec101/CIec101.cpp
    src/protocols/iec101/CIec101.h
    src/protocols/iec101/protocolPcI.cpp
    src/protocols/iec101/protocolPcI.h
    src/protocols/modbus/protocolModbus.cpp
    src/protocols/modbus/protocolModbus.h
    src/protocols/modbus/protocolPcM.cpp
    src/protocols/modbus/protocolPcM.h
    src/protocols/standart/protocolBspS.cpp
    src/protocols/standart/protocolBspS.h
    src/protocols/standart/protocolPcS.cpp
    src/protocols/standart/protocolPcS.h
    src/protocols/standart/protocolS.cpp
    src/protocols/standart/protocolS.h
)



