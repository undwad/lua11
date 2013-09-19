
## CMakeLists.txt 2013.09.19 15.45.19 undwad
## lua11 is a very lightweight binding lua with C++11
## https://github.com/lua11
## see copyright notice in lua11.h

cmake_minimum_required(VERSION 2.6)

set (PROJECT example)
project (${PROJECT})

ADD_DEFINITIONS("-std=c++0x")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	ADD_DEFINITIONS("-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()

include(CheckTypeSize)
CHECK_TYPE_SIZE(void* SIZEOF_PTR)
message("size of pointer = ${SIZEOF_PTR}")

if(SIZEOF_PTR EQUAL 4)
	message("32 bit system")
endif()

if(SIZEOF_PTR EQUAL 8)
	message("64 bit system")
endif()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fpermissive")

file (GLOB HEADER_FILES
	"*.h"
	"*.hpp"
)

file (GLOB SOURCE_FILES
	"*.c"
	"*.cc"
	"*.cpp"
)

include_directories("${CMAKE_SOURCE_DIR}/lua11/lua")
include_directories("${CMAKE_SOURCE_DIR}/lua11")
include_directories("${CMAKE_SOURCE_DIR}")

add_executable(${PROJECT} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries (${PROJECT})


