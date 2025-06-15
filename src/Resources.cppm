module;
#include <span>
#include <string>
#include <cstdint>
#include "stb/stb_image.h"
export module Resources;
export import obscure.vulkan.shader;

export namespace resources
{
#pragma region textures

    std::span<const stbi_uc> viking_room_png() noexcept;

#pragma endregion

#pragma region objects

    const std::string & viking_room_obj();

#pragma endregion

#pragma region shaders
    enum class shader_name {
        floor_vertex,
        floor_fragment,
    };

    template<shader_name shader>
    std::span<const uint32_t> get_data() = delete;

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_vertex>();

    template<>
    std::span<const uint32_t> get_data<shader_name::floor_fragment>();
#pragma endregion
}

export
template<resources::shader_name shader>
struct shader_loader<shader> {
    static obscure::vulkan::shader_data auto load_shader() {
        return resources::get_data<shader>();
    }
};




