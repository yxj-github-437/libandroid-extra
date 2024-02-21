project(c_extra LANGUAGES C CXX ASM)

set(SRC_LIST
    src/async_safe_log.cpp
    src/bionic_futex.cpp
    src/glob.c
    src/conio.cpp
    src/grp.c
	src/hcreate.c
	src/hcreate_r.c
	src/hdestroy_r.c
	src/hsearch_r.c
    src/ifaddrs.c
    src/langinfo.c
    src/nl_types.c
    src/posix_spawn.cpp
    src/pthread_barrier.cpp
    src/pthread_spinlock.cpp
    src/pty.c
    src/stdio.cpp
    src/stdio_ext.c
    src/stdio_fmemopen.c
    src/string.c
    src/stdio_open_memstream.c
    src/whar.c
    src/execinfo.cpp
    src/memfd_create.c
    src/mblen.c
    src/semaphore.c
    src/lockf.c
    src/getcomtext.S
    src/makecontext.c
    src/setcontext.S
    src/startcontext.S
    src/swapcontext.S)

add_library(${PROJECT_NAME} ${SRC_LIST})


if(STRIP_LIBRARY)
    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_STRIP} -R.comment --strip-unneeded "$<TARGET_FILE:${PROJECT_NAME}>"
                   COMMENT "Strip unneeded symbols done on final binary.")
endif()
