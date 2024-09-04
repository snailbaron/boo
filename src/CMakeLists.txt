set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    add_compile_definitions(NOMINMAX)
endif()

add_subdirectory(arg)

add_subdirectory(boo)
add_subdirectory(packer)
add_subdirectory(schema)
add_subdirectory(sdl)