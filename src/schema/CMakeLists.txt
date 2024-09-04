add_custom_command(
    COMMENT "generating header for data flatbuffers"
    DEPENDS schema.fbs
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/schema_generated.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND "$<TARGET_FILE:flatc>"
        --cpp
        --scoped-enums
        -o "${CMAKE_CURRENT_BINARY_DIR}/include"
        schema.fbs
)

add_library(schema
    schema.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/include/schema_generated.h"
)
target_include_directories(schema PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(schema PUBLIC flatbuffers)