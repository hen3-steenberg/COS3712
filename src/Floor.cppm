module;
#include <cmath>
#include <vulkan/vulkan.hpp>
#define GLM_ALIGNED_TYPEDEF
#include <glm/glm.hpp>
export module Floor;
export import obscure.vulkan.pipeline;
export import Resources;
import GlobalState;

export struct Floor {
    using shader_list = obscure::make_set<
        resources::shader_name::floor_vertex,
        resources::shader_name::floor_fragment
    >;

    struct push_constant_t {
        glm::mat4 transform;
        glm::vec2 position;
    };

    static obscure::vulkan::static_pipeline_builder<2, 2, 0, 0> initialize(
        vk::Device device,
        vk::RenderPass render_pass,
        vk::SampleCountFlagBits samples,
        std::array<vk::ShaderModule, 2> const &shaders) {
#pragma region shaders_fixed_functions
        auto result = obscure::vulkan::default_pipeline_builder<0, 0,
            vk::PrimitiveTopology::eTriangleList,
            vk::PolygonMode::eFill,
            vk::FrontFace::eClockwise,
            vk::ShaderStageFlagBits::eVertex,
            vk::ShaderStageFlagBits::eFragment
        >(render_pass, samples, shaders, {}, {});

#pragma endregion

#pragma region push constants
        vk::PushConstantRange push_constants{
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(push_constant_t)
        };
#pragma endregion

#pragma region pipeline_layout
        vk::PipelineLayoutCreateInfo pipeline_info{
            {},
            0,
            nullptr,
            1,
            &push_constants
        };

        result.layout = device.createPipelineLayout(pipeline_info);
#pragma endregion
        return result;
    }

    template<float step>
    static float step_pos(float a) {
        constexpr static float modifier = 2.0f * step;
        a /= modifier;
        return std::floor(a) * modifier;
    }

    struct draw_calls : obscure::vulkan::draw_call_base {
        void draw_floor(glm::mat4 transform) const {
            bind_pipeline();

            vk::Viewport viewport{
                0.0f,
                0.0f,
                static_cast<float>(get_extent().width),
                static_cast<float>(get_extent().height),
                0.0f,
                1.0f
            };

            get_command_buffer().setViewport(0, 1, &viewport);

            vk::Rect2D scissor{
                {0, 0},
                get_extent()
            };

            get_command_buffer().setScissor(0, 1, &scissor);


            push_constant_t push_const{
                transform,
                glm::vec2 {
                    step_pos<0.5f>(global::cameraPosition().x),
                    step_pos<0.5f>(global::cameraPosition().y)
                }
            };


            get_command_buffer().pushConstants(get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(push_constant_t), &push_const);

            get_command_buffer().draw(6000000, 1, 0, 0);
        }
    };
};

static_assert(obscure::vulkan::pipeline_definition<Floor>);
