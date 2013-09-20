
## CMakeLists.txt 2013.09.20 09.56.52 undwad
## lua11 is a very lightweight binding lua with C++11
## https://github.com/undwad/lua11 mailto:undwad@mail.ru
## see copyright notice in lua11.h

cmake_minimum_required(VERSION 2.6)

set (PROJECT example.exe)
project (${PROJECT})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	ADD_DEFINITIONS("-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	ADD_DEFINITIONS("-std=c++0x")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()

file (GLOB FILES
	"lua11/lua/*.h"
	"lua11/lua/*.c"
	"lua11/*.h"
	"*.h"
	"*.cpp"
)

add_executable(${PROJECT} ${FILES})

target_link_libraries (${PROJECT})


