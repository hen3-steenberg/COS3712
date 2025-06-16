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

#pragma endregion

#pragma region shaders
    enum class shader_name {
        floor_vertex,
        floor_fragment,
        building_vertex,
        building_fragment
    };

    template<shader_name shader>
    std::span<const uint32_t> get_data() = delete;

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_vertex>();

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_fragment>();

    template<>
    std::span<const uint32_t> get_data<shader_name::building_vertex>();

    template<>
    std::span<const uint32_t> get_data<shader_name::building_fragment>();
#pragma endregion
}

export
template<resources::shader_name shader>
struct shader_loader<shader> {
    static obscure::vulkan::shader_data auto load_shader() {
        return resources::get_data<shader>();
    }
};




