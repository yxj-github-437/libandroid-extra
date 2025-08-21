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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

option(STRIP_LIBRARY "Strip library" ON)
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(libc-extra)
add_subdirectory(libm-extra)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

### check arch
file(WRITE ${PROJECT_BINARY_DIR}/conftest.h
    "#if defined(__aarch64__)
     CMAKE-ARCH-DETECT: ARM64
     #elif defined(__arm__)
     CMAKE-ARCH-DETECT: ARM
     #elif defined(__i386__)
     CMAKE-ARCH-DETECT: X86
     #elif defined(__x86_64__)
     CMAKE-ARCH-DETECT: X86_64
     #endif")

if(CMAKE_C_COMPILER_TARGET)
    set(FFI_ARCH_TEST_FLAGS "--target=${CMAKE_C_COMPILER_TARGET}")
endif()

execute_process(
    COMMAND
    ${CMAKE_C_COMPILER}
    ${FFI_ARCH_TEST_FLAGS}
    -E ${PROJECT_BINARY_DIR}/conftest.h
    -o -
    OUTPUT_VARIABLE _arch_out
    ERROR_VARIABLE _arch_err
    RESULT_VARIABLE _arch_res
    ERROR_STRIP_TRAILING_WHITESPACE)

if(_arch_res EQUAL 0)
    string(REGEX REPLACE ".*CMAKE-ARCH-DETECT: (.+)\n.*" "\\1" TARGET ${_arch_out})
endif()

file(REMOVE ${PROJECT_BINARY_DIR}/conftest.h)

if(NOT TARGET)
    message(FATAL_ERROR "cannot detect target arch.")
endif()

if("${TARGET}" STREQUAL "ARM64")
  set(TARGET_TRIPLE "aarch64-linux-android")
elseif("${TARGET}" STREQUAL "ARM")
  set(TARGET_TRIPLE "arm-linux-androideabi")
elseif("${TARGET}" STREQUAL "X86_64")
  set(TARGET_TRIPLE "x86_64-linux-android")
elseif("${TARGET}" STREQUAL "X86")
  set(TARGET_TRIPLE "i686-linux-android")
endif()

message(STATUS "target ${TARGET_TRIPLE}")


## install
include(GNUInstallDirs)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/sysroot/usr/include
        DESTINATION ${CMAKE_INSTALL_PREFIX}/sysroot/usr
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/sysroot/usr/lib/${TARGET_TRIPLE}
          DESTINATION ${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib)
install(TARGETS c_extra m_extra
        DESTINATION ${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE})

install(CODE "set(TARGET_TRIPLE \"${TARGET_TRIPLE}\")")
install(CODE [[
    file(RENAME $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libc.so
        $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libc_real.so)
    file(RENAME $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libm.so
        $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libm_real.so)

    file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libc.so
        "INPUT(libc_extra.a libc_real.so)")
    file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libm.so
        "INPUT(libm_extra.a libm_real.so)")
    file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libpthread.a
        "INPUT(-lc)")
    file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/librt.so
        "INPUT(-lc)")
    file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sysroot/usr/lib/${TARGET_TRIPLE}/libutil.so
        "INPUT(-lc)")
]])
