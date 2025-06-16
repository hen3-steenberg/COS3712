add_subdirectory("rapidobj")
add_subdirectory("glm")
add_subdirectory("VulkanMemoryAllocator")
add_subdirectory("glfw")

file(GLOB GL_NOISE_SRC "glNoise/src/*.glsl")
file(COPY ${GL_NOISE_SRC} DESTINATION "${PROJECT_SOURCE_DIR}/shaders/glNoise")
