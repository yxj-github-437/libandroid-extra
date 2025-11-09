project(libiconv)

add_library(iconv STATIC
    lib/iconv.c)
target_link_libraries(iconv PUBLIC c_extra)
target_include_directories(iconv PUBLIC include)

if(STRIP_LIBRARY)
    add_custom_command(TARGET iconv POST_BUILD
        COMMAND ${CMAKE_STRIP} -R.comment --strip-unneeded "$<TARGET_FILE:iconv>"
        COMMENT "Strip unneeded symbols done on final binary.")
endif()
