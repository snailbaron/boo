set(unpacked "${PROJECT_BINARY_DIR}/unpacked")
set(packed "${PROJECT_BINARY_DIR}/packed")

set(images
    images/ball.aseprite
    images/brick.aseprite
    images/platform.aseprite
)

add_custom_command(
    COMMENT "generating spritesheet from aseprite files"
    DEPENDS
        aseprite-external
        ${images}
    OUTPUT
        "${unpacked}/images/sheet.json"
        "${unpacked}/images/sheet.png"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND "$<TARGET_FILE:aseprite>"
        --batch
        --data "${unpacked}/images/sheet.json"
        --format json-array
        --sheet "${unpacked}/images/sheet.png"
        --sheet-type packed
        --filename-format "{title}:{tag}:{tagframe}"
        ${images}
)

add_custom_command(
    COMMENT "packing resources into a data file"
    DEPENDS
        "${unpacked}/images/sheet.json"
        "${unpacked}/images/sheet.png"
    OUTPUT
        "${packed}/boo.data"
        "${packed}/include/r.hpp"
    COMMAND "$<TARGET_FILE:packer>"
        --source "${unpacked}"
        --data "${packed}/boo.data"
        --header "${packed}/include/r.hpp"
)

add_custom_target(pack-assets
    COMMENT "running generate spritesheet target"
    DEPENDS
        "${packed}/boo.data"
        "${packed}/include/r.hpp"
)

add_library(resource-ids INTERFACE)
add_dependencies(resource-ids pack-assets)
target_include_directories(resource-ids INTERFACE "${packed}/include")