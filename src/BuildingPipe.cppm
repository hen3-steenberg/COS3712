module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <string>

#include <glm/ext/matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
export module Building;

export import obscure.vulkan.shader;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.buffer;

import Resources;

export struct BuildingObj {
    struct vertex_t {
        glm::vec3 position;
        glm::vec3 normal;
    };

    glm::mat4 model;
    obscure::vulkan::vertex_buffer<vertex_t> vertex_buffer;
    obscure::vulkan::index_buffer<uint32_t> index_buffer;

    template<typename Ctx>
    static BuildingObj load_from_memory(Ctx const& ctx, std::string const& obj, std::string const& mtl) {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.triangulate = true;
        tinyobj::ObjReader reader;
        reader.ParseFromString(obj, mtl, reader_config);

        std::vector<vertex_t> vertices;
        std::vector<uint32_t> indices;

        for (auto & shape : reader.GetShapes()) {
            for (auto indexes : shape.mesh.indices) {
                vertices.emplace_back(vertex_t{
                    .position = {
                        reader.GetAttrib().vertices[3 * indexes.vertex_index + 0],
                        reader.GetAttrib().vertices[3 * indexes.vertex_index + 1],
                        reader.GetAttrib().vertices[3 * indexes.vertex_index + 2]
                    },
                    .normal = {
                        reader.GetAttrib().normals[3 * indexes.normal_index + 0],
                        reader.GetAttrib().normals[3 * indexes.normal_index + 1],
                        reader.GetAttrib().normals[3 * indexes.normal_index + 2]
                    }
                });

                indices.emplace_back(indices.size());
            }
        }

        return BuildingObj{
            .model = glm::identity<glm::mat4>(),
            .vertex_buffer = ctx.template init_vertex_buffer<vertex_t>(vertices),
            .index_buffer = ctx.template init_index_buffer<uint32_t>(indices)
        };
    }
};


export struct BuildingPipe {
    using shader_list = obscure::make_set<
        resources::shader_name::building_vertex,
        resources::shader_name::building_fragment
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
                    sizeof(BuildingObj::vertex_t),
                    vk::VertexInputRate::eVertex
                }
            },
            {
                vk::VertexInputAttributeDescription{
                    0,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(BuildingObj::vertex_t, position)
                },
                vk::VertexInputAttributeDescription{
                    1,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(BuildingObj::vertex_t, normal)
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
        void draw_building(glm::mat4 world, BuildingObj const &building) const {
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

            vk::Buffer buffers[] = {building.vertex_buffer.get_buffer()};
            vk::DeviceSize offsets[] = {0};

            get_command_buffer().bindVertexBuffers(0, 1, buffers, offsets);

            get_command_buffer().bindIndexBuffer(building.index_buffer.get_buffer(), 0,
                                                 building.index_buffer.get_index_type());

            glm::mat4 transform = world * building.model;

            get_command_buffer().pushConstants(get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0,
                                               sizeof(glm::mat4), &transform);

            get_command_buffer().drawIndexed(building.index_buffer.count(), 1, 0, 0, 0);
        }
    };
};

static_assert(obscure::vulkan::pipeline_definition<BuildingPipe>);
