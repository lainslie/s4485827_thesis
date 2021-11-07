# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(thesis)


set(MY_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../../mylib)

include_directories(include)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
  message(WARNING "hi ${dirlist}")
ENDMACRO()

#set(DIRECTORY_LIST "")

SUBDIRLIST(DIRECTORY_LIST ${MY_LIB})

foreach(dir ${DIRECTORY_LIST})
    include_directories(${MY_LIB}/${dir}/include)
    file(GLOB_RECURSE SRCS ${MY_LIB}/${dir}/src/*.c)
    foreach(source ${SRCS})
        target_sources(app PRIVATE ${source})
    endforeach()
endforeach()


add_definitions(-DPROJECT)


target_sources(app PRIVATE ${proto_sources} src/main.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)