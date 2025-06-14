#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>
#include <filesystem>
#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

import obscure.vulkan;
import obscure.utils.stopwatch;
import obscure.builtin.pipelines.color_2d;
import obscure.builtin.pipelines.texture_3d2d;
import obscure.imgui;
import GlobalState;
import MenuOverlay;
import Resources;
import Camera;

using pipeline_t = obscure::builtin::pipeline::texture_3d2d;
using gfx_ctx_t = obscure::graphics_context<pipeline_t>;
using vertex_t = obscure::builtin::pipeline::texture_3d2d_vertex;

struct object_3d
{
	obscure::vulkan::vertex_buffer<vertex_t> vertex_buffer;
	obscure::vulkan::index_buffer<uint32_t> index_buffer;
	obscure::vulkan::rgba_2d_texture<> color_texture;

	static object_3d load(gfx_ctx_t const& ctx)
	{


		tinyobj::ObjReader reader;
		tinyobj::ObjReaderConfig config{};
		config.triangulate = true;
		if (!reader.ParseFromString(viking_room_obj(), "", config))
		{
			throw std::runtime_error("Failed to parse obj file");
		}

		std::vector<vertex_t> vertices;
		std::vector<uint32_t> indices;

		for (auto const& shape : reader.GetShapes())
		{
			for (auto index : shape.mesh.indices)
			{
				vertices.emplace_back(vertex_t {
					{
						reader.GetAttrib().vertices[3 * index.vertex_index + 0],
						reader.GetAttrib().vertices[3 * index.vertex_index + 1],
						reader.GetAttrib().vertices[3 * index.vertex_index + 2]
					},
					{
						reader.GetAttrib().texcoords[2 * index.texcoord_index + 0],
						1.0f - reader.GetAttrib().texcoords[2 * index.texcoord_index + 1]
					}
				});

				indices.emplace_back(indices.size());
			}
		}

		return object_3d {
			ctx.init_vertex_buffer<vertex_t>(vertices),
			ctx.init_index_buffer<uint32_t>(indices),
			ctx.load_texture<pipeline_t>(viking_room_png(), 0)
		};
	}
};


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});




	{
		gfx_ctx_t app{};
		global::windowRef() = app.window.get_window_ref();
		obscure::imgui::ctx imgui_ctx{app};
		object_3d model_3d = object_3d::load(app);


		obscure::stopwatch<float> frame_timer{};
		while (!app.window.should_close())
		{
			glfwPollEvents();
			if (app.window.isKeyPressed(GLFW_KEY_ESCAPE) || MenuExit())
			{
				break;
			}
			{
				auto frame = app.begin_frame();
				{
					auto extent = frame.get_extent();

					float time = frame_timer.total_time().count();
					glm::mat4 model = glm::identity<glm::mat4>(); //glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					glm::mat4 proj = glm::perspective(glm::radians(60.0f), extent.width / (float) extent.height, 0.1f, 100.0f);
					proj[1][1]*= -1;

					glm::mat4 viewproj = proj * GetCameraTransform();

					frame.draw_texture_2d(viewproj, model, model_3d.vertex_buffer, model_3d.index_buffer, model_3d.color_texture);
				}
				drawMenuOverlay(frame.get_command_buffer());
			}
			app.submit_frame();
			app.draw_frame();
		}
	}
	obscure::free_instance();

	return 0;
}