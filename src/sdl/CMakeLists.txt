add_library(sdl
    sdl.cpp
)
target_include_directories(sdl PUBLIC include)
target_link_libraries(sdl
    PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
    INTERFACE SDL2::SDL2main
)