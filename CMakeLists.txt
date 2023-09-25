cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project(libandroid-extra)

set(CMAKE_SYSROOT "${PROJECT_SOURCE_DIR}/sysroot")

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=gnu++11 -fPIC -fno-exceptions -fno-rtti")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=gnu++23 -fPIC -fno-exceptions -fno-rtti")
endif()
set(CMAKE_C_FLAGS "-std=gnu11 -fPIC")

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

option(STRIP_LIBRARY "Strip library" ON)
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(libc-extra)
add_subdirectory(libm-extra)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
