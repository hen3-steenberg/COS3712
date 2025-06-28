module;
#include <span>
#include <cstdint>
#include <string_view>
#include "stb/stb_image.h"
export module Resources;
export import obscure.vulkan.shader;

export namespace resources
{
#pragma region textures

    std::span<const stbi_uc> viking_room_png() noexcept;

#pragma endregion

#pragma region objects

    std::span<const char> viking_room_obj();

    std::span<const char> portal_obj();

    std::string_view portal_mtl();

    std::span<const char> hole_building_obj();

    std::string_view hole_building_mtl();

    std::span<const char> ship_obj();

    std::string_view ship_mtl();

    std::span<const char> drone_obj();

    std::string_view drone_mtl();

    std::span<const char> building1_obj();

    std::string_view building1_mtl();

    std::span<const char> building2_obj();

    std::string_view building2_mtl();

    std::span<const char> building3_obj();

    std::string_view building3_mtl();

#pragma endregion

#pragma region shaders
    enum class shader_name {
        floor_vertex,
        floor_fragment,
        object_vertex,
        object_fragment
    };

    template<shader_name shader>
    std::span<const uint32_t> get_data() = delete;

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_vertex>();

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_fragment>();

    template<>
    std::span<const uint32_t> get_data<shader_name::object_vertex>();

    template<>
    std::span<const uint32_t> get_data<shader_name::object_fragment>();
#pragma endregion
}

export
template<resources::shader_name shader>
struct shader_loader<shader> {
    static obscure::vulkan::shader_data auto load_shader() {
        return resources::get_data<shader>();
    }
};




