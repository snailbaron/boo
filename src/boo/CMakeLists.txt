set(EXE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(DATA_FILE "${PROJECT_BINARY_DIR}/packed/boo.data")
configure_file(build-info.hpp.in include/build-info.hpp @ONLY)

add_executable(boo
    main.cpp
 "window.cpp" "mmap.cpp" "resources.cpp" "timer.cpp" "world.cpp" "geometry.cpp" "collision.cpp" "view.cpp")
target_link_libraries(boo PRIVATE sdl resource-ids schema)
target_include_directories(boo PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")

add_custom_command(TARGET boo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:boo> $<TARGET_RUNTIME_DLLS:boo>
    COMMAND_EXPAND_LISTS
)