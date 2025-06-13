cmake_minimum_required (VERSION 3.31)

project ("COS3712" CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("obscure")



add_executable(cos3712)

target_sources(cos3712
    PRIVATE
        "src/cos3712.cpp"
        "src/resources/resources.cxx"
    PUBLIC FILE_SET
    CXX_MODULES FILES
        "src/menu_overlay.cppm"
        "src/resources.cppm"
)
target_link_libraries(cos3712 PRIVATE obscure)
target_link_libraries(cos3712 PRIVATE imgui)

