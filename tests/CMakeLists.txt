project(tests 
        LANGUAGES C)

add_executable(fmemopen_test src/fmemopen_test.c)
target_link_libraries(fmemopen_test c_extra)

add_executable(barrier_test src/barrier_test.c)
target_link_libraries(barrier_test c_extra)

add_executable(posix_spawn_test src/posix_spawn_test.c)
target_link_libraries(posix_spawn_test c_extra)

add_executable(open_memstream_test src/open_memstream_test.c)
target_link_libraries(open_memstream_test c_extra)

add_executable(spinlock_test src/spinlock_test.c)
target_link_libraries(spinlock_test c_extra)

add_executable(spinlock_test2 src/spinlock_test2.c)
target_link_libraries(spinlock_test2 c_extra)

add_executable(conio_test src/conio_test.c)
target_link_libraries(conio_test c_extra)

add_executable(lockf_test src/lockf_test.c)
target_link_libraries(lockf_test c_extra)

add_executable(sem_test src/sem_test.c)
target_link_libraries(sem_test c_extra)

add_executable(execinfo_test src/execinfo_test.c)
target_link_libraries(execinfo_test c_extra)
set_target_properties(execinfo_test PROPERTIES LINK_FLAGS "-rdynamic")


add_executable(complex_test src/complex_test.c)
target_link_libraries(complex_test m_extra m)
