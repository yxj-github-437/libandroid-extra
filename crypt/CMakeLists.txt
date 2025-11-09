project(libcrypt)

add_library(crypt STATIC
    src/crypt_blowfish.c
    src/crypt.c
    src/crypt_des.c
    src/crypt_md5.c
    src/crypt_r.c
    src/crypt_sha256.c
    src/crypt_sha512.c
    src/encrypt.c)

target_include_directories(crypt PUBLIC
    include)

if(STRIP_LIBRARY)
    add_custom_command(TARGET crypt POST_BUILD
        COMMAND ${CMAKE_STRIP} -R.comment --strip-unneeded "$<TARGET_FILE:crypt>"
        COMMENT "Strip unneeded symbols done on final binary.")
endif()
