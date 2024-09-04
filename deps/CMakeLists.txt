include(ExternalProject)

add_subdirectory(flatbuffers)
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)
add_subdirectory(yaml-cpp)

ExternalProject_Add(aseprite-external
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/aseprite"
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(aseprite-external BINARY_DIR)
add_executable(aseprite IMPORTED GLOBAL)
set_target_properties(aseprite
    PROPERTIES IMPORTED_LOCATION "${BINARY_DIR}/bin/aseprite")