cmake_minimum_required (VERSION 3.31)

project ("COS3712" CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OBSCURE_TEST OFF)

add_subdirectory("3rd_party")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")
add_subdirectory("obscure")
add_subdirectory("shaders")
add_subdirectory("object_seperate")


add_executable(cos3712)

target_sources(cos3712
    PRIVATE
        "src/cos3712.cpp"
        "src/resources/Resources.cxx"
    PUBLIC FILE_SET
    CXX_MODULES FILES
        "src/GlobalState.cppm"
        "src/MenuOverlay.cppm"
        "src/Resources.cppm"
        "src/Camera.cppm"
        "src/Floor.cppm"
        "src/ObjectPipe.cppm"
        "src/Building.cppm"
        "src/Vehicle.cppm"
        "src/Animation.cppm"
        "src/Drone.cppm"
)
target_link_libraries(cos3712 PRIVATE obscure::obscure)
target_link_libraries(cos3712 PRIVATE obscure::imgui)
target_link_libraries(cos3712 PRIVATE rapidobj::rapidobj)
add_dependencies(cos3712 custom_shaders)

find_program(CLANG_FORMAT_EXE clang-format)
file(GLOB_RECURSE CXX_FILES "*.cppm" "*.cxx" "*.cpp" "*.hpp")
list(FILTER CXX_FILES EXCLUDE REGEX "3rd_party")
list(FILTER CXX_FILES EXCLUDE REGEX "obscure")
file(GLOB_RECURSE SHADER_FILES "*.vert" "*.frag")
list(FILTER SHADER_FILES EXCLUDE REGEX "3rd_party")
list(FILTER SHADER_FILES EXCLUDE REGEX "obscure")

if(CLANG_FORMAT_EXE)
    add_custom_target(FORMAT_COS3712
            COMMENT "Formatting files with clang-format..."
            COMMAND ${CLANG_FORMAT_EXE} -i ${CXX_FILES} ${SHADER_FILES}
    )
    message(STATUS "Added 'format' target")
else()
    message(WARNING
            "clang-format not found - format target unavailable")
endif()



