module;
#include <span>
#include <cstdint>
#include <bit>
#include <string_view>
#include "stb/stb_image.h"
module Resources;
using namespace resources;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc23-extensions"

#pragma region textures
constexpr const char viking_room_png_data[] = {
#embed "textures/viking_room.png"
};

std::span<const stbi_uc> resources::viking_room_png() noexcept
{
    return std::span<const stbi_uc>{ reinterpret_cast<stbi_uc const*>(viking_room_png_data), sizeof(viking_room_png_data) / sizeof(stbi_uc) };
}

#pragma endregion

#pragma region objects

constexpr const char viking_room_obj_data[] = {
#embed "objects/viking_room.obj"
};


std::span<const char> resources::viking_room_obj()
{
    return std::span<const char>{ viking_room_obj_data };
}

constexpr const char portal_building_obj_data[] = {
#embed "objects/portal_building.obj"
};

std::span<const char> resources::portal_obj() {
    return std::span<const char> { portal_building_obj_data };
}

constexpr const char portal_building_mtl_data[] = {
#embed "objects/portal_building.mtl"
};

std::string_view resources::portal_mtl() {
    return std::string_view{ portal_building_mtl_data };
}

constexpr const char hole_building_obj_data[] = {
#embed "objects/hole_building.obj"
};

std::span<const char> resources::hole_building_obj() {
    return std::span<const char>{ hole_building_obj_data };
}

constexpr const char hole_building_mtl_data[] = {
#embed "objects/hole_building.mtl"
};

std::string_view resources::hole_building_mtl() {
    return std::string_view{ hole_building_mtl_data };
}

constexpr const char ship_obj_data[] = {
#embed "objects/ship.obj"
};

std::span<const char> resources::ship_obj() {
    return std::span<const char>{ ship_obj_data };
}

constexpr const char ship_mtl_data[] = {
#embed "objects/ship.mtl"
};

std::string_view resources::ship_mtl() {
    return std::string_view{ ship_mtl_data };
}


constexpr const char drone_obj_data[] = {
#embed "objects/drone.obj"
};

std::span<const char> resources::drone_obj() {
    return std::span<const char>{ drone_obj_data };
}

constexpr const char drone_mtl_data[] = {
#embed "objects/drone.mtl"
};

std::string_view resources::drone_mtl() {
    return std::string_view{ drone_mtl_data };
}
#pragma endregion

#pragma region shaders

constexpr size_t get_actual_size(size_t size)
{
    if (size % 4)
    {
        return size / 4;
    }
    else
    {
        return (size - 4) / 4;
    }
}

template<std::size_t N>
std::span<const uint32_t> get_shader_data(const uint8_t (&data)[N]) {
    return std::span<const uint32_t>{reinterpret_cast<const uint32_t*>(data), get_actual_size(N)};
}

constexpr const uint8_t floor_vertex_data[] = {
#embed "shaders/floor.vert.spv"
    , 0, 0, 0, 0
};

template<>
std::span<const uint32_t> resources::get_data<shader_name::floor_vertex>() {
    return get_shader_data(floor_vertex_data);
}

constexpr const uint8_t floor_fragment_data[] = {
#embed "shaders/floor.frag.spv"
    , 0, 0, 0, 0
};

template<>
std::span<const uint32_t> resources::get_data<shader_name::floor_fragment>() {
    return get_shader_data(floor_fragment_data);
}

constexpr const uint8_t object_vertex_data[] = {
#embed "shaders/object.vert.spv"
    , 0, 0, 0, 0
};

template<>
std::span<const uint32_t> resources::get_data<shader_name::object_vertex>() {
    return get_shader_data(object_vertex_data);
}

constexpr const uint8_t object_fragment_data[] = {
#embed "shaders/object.frag.spv"
    , 0, 0, 0, 0
};

template<>
std::span<const uint32_t> resources::get_data<shader_name::object_fragment>() {
    return get_shader_data(object_fragment_data);
}

#pragma endregion



#pragma clang diagnostic pop