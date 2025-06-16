module;
#include <vulkan/vulkan.hpp>
#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <string_view>
#include <spanstream>
#include <span>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "rapidobj/rapidobj.hpp"
export module ObjectPipe;

export import obscure.vulkan.shader;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.buffer;

import Resources;

export struct ObjectModel {
    struct vertex_t {
        glm::vec3 position;
        glm::vec3 normal;
    };

    obscure::vulkan::vertex_buffer<vertex_t> vertex_buffer;
    obscure::vulkan::index_buffer<uint32_t> index_buffer;

    template<typename Ctx>
    static ObjectModel load_from_memory(Ctx const& ctx, std::span<const char> obj, std::string_view mtl) {
        std::ispanstream ss{obj};
        auto Parsed_obj = rapidobj::detail::ParseStream(ss, rapidobj::MaterialLibrary::String(mtl));
        //rapidobj::Triangulate(Parsed_obj);

        std::vector<vertex_t> vertices;
        std::vector<uint32_t> indices;

        for (auto & shape : Parsed_obj.shapes) {
            for (auto indexes : shape.mesh.indices) {
                vertices.emplace_back(vertex_t{
                    .position = {
                        Parsed_obj.attributes.positions[3 * indexes.position_index + 0],
                        Parsed_obj.attributes.positions[3 * indexes.position_index + 1],
                        Parsed_obj.attributes.positions[3 * indexes.position_index + 2]
                    },
                    .normal = {
                        Parsed_obj.attributes.normals[3 * indexes.normal_index + 0],
                        Parsed_obj.attributes.normals[3 * indexes.normal_index + 1],
                        Parsed_obj.attributes.normals[3 * indexes.normal_index + 2]
                    }
                });

                indices.emplace_back(indices.size());
            }
        }

        return ObjectModel{
            .vertex_buffer = ctx.template init_vertex_buffer<vertex_t>(vertices),
            .index_buffer = ctx.template init_index_buffer<uint32_t>(indices)
        };
    }
};


export struct ObjectPipe {
    using shader_list = obscure::make_set<
        resources::shader_name::object_vertex,
        resources::shader_name::object_fragment
    >;

    static obscure::vulkan::static_pipeline_builder<2, 2, 1, 2> initialize(
        vk::Device device,
        vk::RenderPass render_pass,
        vk::SampleCountFlagBits samples,
        std::array<vk::ShaderModule, 2> const &shaders) {
#pragma region shaders_fixed_functions
        auto result = obscure::vulkan::default_pipeline_builder<1, 2,
            vk::PrimitiveTopology::eTriangleList,
            vk::PolygonMode::eFill,
            vk::FrontFace::eCounterClockwise,
            vk::ShaderStageFlagBits::eVertex,
            vk::ShaderStageFlagBits::eFragment>
        (
            render_pass,
            samples,
            shaders,
            {
                vk::VertexInputBindingDescription{
                    0,
                    sizeof(ObjectModel::vertex_t),
                    vk::VertexInputRate::eVertex
                }
            },
            {
                vk::VertexInputAttributeDescription{
                    0,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(ObjectModel::vertex_t, position)
                },
                vk::VertexInputAttributeDescription{
                    1,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(ObjectModel::vertex_t, normal)
                }
            });

#pragma endregion

#pragma region push_constants
        vk::PushConstantRange push_constants{
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(glm::mat4)
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

    struct draw_calls : obscure::vulkan::draw_call_base {
        void draw_object(glm::mat4 world_transform, glm::mat4 model_transform, ObjectModel const &object) const {
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

            vk::Buffer buffers[] = {object.vertex_buffer.get_buffer()};
            vk::DeviceSize offsets[] = {0};

            get_command_buffer().bindVertexBuffers(0, 1, buffers, offsets);

            get_command_buffer().bindIndexBuffer(object.index_buffer.get_buffer(), 0,
                                                 object.index_buffer.get_index_type());

            glm::mat4 transform = world_transform * model_transform;

            get_command_buffer().pushConstants(get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0,
                                               sizeof(glm::mat4), &transform);

            get_command_buffer().drawIndexed(object.index_buffer.count(), 1, 0, 0, 0);
        }

        void draw_objects(glm::mat4 world_transform, std::span<const glm::mat4> model_transforms, ObjectModel const &object) const {
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

            vk::Buffer buffers[] = {object.vertex_buffer.get_buffer()};
            vk::DeviceSize offsets[] = {0};

            get_command_buffer().bindVertexBuffers(0, 1, buffers, offsets);

            get_command_buffer().bindIndexBuffer(object.index_buffer.get_buffer(), 0,
                                                 object.index_buffer.get_index_type());

            for (auto const& model_transform : model_transforms) {
                glm::mat4 transform = world_transform * model_transform;

                get_command_buffer().pushConstants(get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0,
                                                   sizeof(glm::mat4), &transform);

                get_command_buffer().drawIndexed(object.index_buffer.count(), 1, 0, 0, 0);
            }
        }
    };
};

static_assert(obscure::vulkan::pipeline_definition<ObjectPipe>);
