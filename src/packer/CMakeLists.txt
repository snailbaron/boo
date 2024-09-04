add_executable(packer
    main.cpp
)
target_link_libraries(packer PRIVATE flatbuffers yaml-cpp::yaml-cpp arg schema)

add_custom_command(TARGET packer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:packer> $<TARGET_RUNTIME_DLLS:packer>
    COMMAND_EXPAND_LISTS
)