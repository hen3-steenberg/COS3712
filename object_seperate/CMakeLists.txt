add_executable(obj_sep)

target_sources(obj_sep
    PRIVATE
        obj_sep.cpp
    PUBLIC FILE_SET
    CXX_MODULES FILES
        read_obj.cppm
        triangle.cppm
        write_obj.cppm
)

target_link_libraries(obj_sep PRIVATE rapidobj::rapidobj)
target_link_libraries(obj_sep PRIVATE glm::glm)
