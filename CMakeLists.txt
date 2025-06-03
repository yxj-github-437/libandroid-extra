cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project(libandroid-extra)

set(CMAKE_SYSROOT "${PROJECT_SOURCE_DIR}/sysroot")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-fno-exceptions -fno-rtti")

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

option(STRIP_LIBRARY "Strip library" ON)
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(libc-extra)
add_subdirectory(libm-extra)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
