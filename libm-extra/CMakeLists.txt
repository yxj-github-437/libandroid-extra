project(m_extra LANGUAGES C)

aux_source_directory(src SRC_LIST)
add_library(${PROJECT_NAME} ${SRC_LIST})

if(STRIP_LIBRARY)
    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_STRIP} -R.comment --strip-unneeded "$<TARGET_FILE:${PROJECT_NAME}>"
                   COMMENT "Strip unneeded symbols done on final binary.")
endif()
