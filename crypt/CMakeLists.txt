project(libcrypt)

add_library(crypt STATIC
    crypt_blowfish.c
    crypt.c
    crypt_des.c
    crypt_md5.c
    crypt_r.c
    crypt_sha256.c
    crypt_sha512.c
    encrypt.c)

if(STRIP_LIBRARY)
    add_custom_command(TARGET crypt POST_BUILD
        COMMAND ${CMAKE_STRIP} -R.comment --strip-unneeded "$<TARGET_FILE:crypt>"
        COMMENT "Strip unneeded symbols done on final binary.")
endif()
