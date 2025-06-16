#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>

import obscure.vulkan;
import obscure.imgui;

import GlobalState;
import MenuOverlay;
import Resources;
import Camera;
import Floor;
import Building;

struct app_t {
	using gfx_ctx_t = obscure::graphics_context<BuildingPipe, Floor>;
	using frame_t = gfx_ctx_t::command_session_t;

	gfx_ctx_t gfx_ctx;
	obscure::imgui::ctx imgui_ctx;
	BuildingObj inPortal;
	BuildingObj outPortal;
	BuildingObj Hole1;
	BuildingObj Hole2;


	app_t()
		: gfx_ctx(),
		imgui_ctx(gfx_ctx),
		inPortal(BuildingObj::load_from_memory(gfx_ctx, resources::portal_obj(), resources::portal_mtl())),
		outPortal(BuildingObj::load_from_memory(gfx_ctx, resources::portal_obj(), resources::portal_mtl())),
		Hole1(BuildingObj::load_from_memory(gfx_ctx, resources::hole_building_obj(), resources::hole_building_mtl())),
		Hole2(BuildingObj::load_from_memory(gfx_ctx, resources::hole_building_obj(), resources::hole_building_mtl()))
	{
		inPortal.rotate_and_move(45.0f, glm::vec3{50.0f, 50.0f, 0.0f});
		outPortal.rotate_and_move(90.0f, glm::vec3{-70.0f, -100.0f, 0.0f});
		Hole1.rotate_and_move(15.0f, glm::vec3{20.0f, -50.0f, 0.0f});
		Hole2.rotate_and_move(20.0f, glm::vec3{-20.0f, 100.0f, 0.0f});
		global::windowRef() = gfx_ctx.window.get_window_ref();
	}

	void loop(frame_t frame) {
#pragma region perspective
		auto extent = frame.get_extent();

		glm::mat4 proj = glm::perspective(glm::radians(60.0f), extent.width / (float) extent.height, 0.1f, 1000.0f);
		proj[1][1]*= -1;

		glm::mat4 viewproj = proj * GetCameraTransform();
#pragma endregion

#pragma region buildings

		frame.draw_building(viewproj, inPortal);
		frame.draw_building(viewproj, outPortal);
		frame.draw_building(viewproj, Hole1);
		frame.draw_building(viewproj, Hole2);

#pragma endregion

#pragma region floor

		frame.draw_floor(viewproj);

#pragma endregion

#pragma region overlay

		drawMenuOverlay(frame.get_command_buffer());

#pragma endregion
	}

	void run() {
		while (!gfx_ctx.window.should_close())
		{
			glfwPollEvents();
			if (gfx_ctx.window.isKeyPressed(GLFW_KEY_ESCAPE) || MenuExit())
			{
				break;
			}
			loop(gfx_ctx.begin_frame());
			gfx_ctx.submit_frame();
			gfx_ctx.draw_frame();
		}
	}
};


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});
	{
		app_t app{};
		app.run();
	}
	obscure::free_instance();

	return 0;
}