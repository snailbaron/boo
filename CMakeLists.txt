cmake_minimum_required(VERSION 3.24)

project(boo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/wd4251)
endif()

add_subdirectory(deps)
add_subdirectory(assets)
add_subdirectory(src)