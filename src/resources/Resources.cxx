module;
#include <span>
#include <string>
#include "stb/stb_image.h"
module Resources;

constexpr const char viking_room_png_data[] = {
#embed "viking_room.png"
};

constexpr const char viking_room_obj_data[] = {
#embed "viking_room.obj"
};



std::span<const stbi_uc> viking_room_png() noexcept
{
    return std::span<const stbi_uc>{ reinterpret_cast<stbi_uc const*>(viking_room_png_data), sizeof(viking_room_png_data) / sizeof(stbi_uc) };
}

const std::string & viking_room_obj()
{
    static const std::string viking_room_obj_str{ viking_room_obj_data };
    return viking_room_obj_str;
}