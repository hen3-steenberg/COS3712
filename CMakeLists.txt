cmake_minimum_required (VERSION 3.31)

project ("COS3712" CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("obscure")
add_subdirectory("shaders")



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
        "src/BuildingPipe.cppm"
)
target_link_libraries(cos3712 PRIVATE obscure)
target_link_libraries(cos3712 PRIVATE imgui)
add_dependencies(cos3712 custom_shaders)

