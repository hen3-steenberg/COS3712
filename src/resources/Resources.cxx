module;
#include <span>
#include <string>
#include <cstdint>
#include <bit>
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


const std::string & resources::viking_room_obj()
{
    static const std::string viking_room_obj_str{ viking_room_obj_data };
    return viking_room_obj_str;
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

#pragma endregion



#pragma clang diagnostic pop