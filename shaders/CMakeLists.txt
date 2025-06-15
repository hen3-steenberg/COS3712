file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/src/resources/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        custom_shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)