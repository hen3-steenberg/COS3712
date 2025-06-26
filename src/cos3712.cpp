#include <GLFW/glfw3.h>
#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>
#include <chrono>
#include <print>

import obscure.vulkan;
import obscure.imgui;

import GlobalState;
import MenuOverlay;
import Resources;
import Camera;
import Floor;
import ObjectPipe;
import Building;
import Vehicle;
import Animation;
import Drone;

using namespace std::literals;

constexpr auto build_ship_animation() {
	auto sequence = (Rotate{glm::vec3 {0.0f, 0.0f, 15.0f}} & 12s) | (Identity{} & 15s);
	return Move{glm::vec3 {0.0f, 10.0f, 0.0f}} & Loop{glm::translate(glm::identity<glm::mat4>(), glm::vec3{0.0f, 0.0f, 20.0f}), sequence | sequence};
}

struct app_t {
	using gfx_ctx_t = obscure::graphics_context<ObjectPipe, Floor>;
	using frame_t = gfx_ctx_t::command_session_t;
	using ship_animation_t = decltype(build_ship_animation());

	gfx_ctx_t gfx_ctx;
	obscure::imgui::ctx imgui_ctx;
	Building portal;
	Building hole;
	Vehicle ship;
	ship_animation_t ship_animation;
	DroneList drones;


	app_t()
		: gfx_ctx(),
		imgui_ctx(gfx_ctx),
		portal(gfx_ctx, resources::portal_obj(), resources::portal_mtl()),
		hole(gfx_ctx, resources::hole_building_obj(), resources::hole_building_mtl()),
		ship(gfx_ctx, resources::ship_obj(), resources::ship_mtl()),
		ship_animation(build_ship_animation()),
		drones(gfx_ctx)
	{
		portal.add_instance(45.0f, glm::vec3{50.0f, 50.0f, 0.0f});
		portal.add_instance(90.0f, glm::vec3{-70.0f, -100.0f, 0.0f});
		hole.add_instance(15.0f, glm::vec3{20.0f, -50.0f, 0.0f});
		hole.add_instance(20.0f, glm::vec3{-20.0f, 100.0f, 0.0f});


		drones.add_drone(glm::vec3{20.0f, 20.0f, 100.0f});
		drones.add_drone(glm::vec3{20.0f, -20.0f, 100.0f});
		drones.add_drone(glm::vec3{-20.0f, 20.0f, 100.0f});
		drones.add_drone(glm::vec3{-20.0f, -20.0f, 100.0f});


		global::windowRef() = gfx_ctx.window.get_window_ref();
	}

	void loop(frame_t frame) {
#pragma region perspective
		auto extent = frame.get_extent();

		glm::mat4 proj = glm::perspective(glm::radians(60.0f), extent.width / (float) extent.height, 0.1f, 1000.0f);
		proj[1][1]*= -1;

		glm::mat4 viewproj = proj * GetCameraTransform();
#pragma endregion

#pragma region drones

		drones.animate();

		frame.draw_objects(viewproj, drones.locations, drones.model);

#pragma endregion

#pragma region vehicles

		ship.transform = evaluate_animations(ship.transform, ship_animation);
		frame.draw_object(viewproj, ship.transform, *ship.model);

#pragma endregion

#pragma region buildings

		frame.draw_objects(viewproj, portal.locations, portal.model);
		frame.draw_objects(viewproj, hole.locations, hole.model);

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
	std::println("The size of the CPU resources is {} bytes.", sizeof(app_t));
	obscure::initialize("Test App", obscure::version{1,0,0});
	{
		app_t app{};
		app.run();
	}
	obscure::free_instance();

	return 0;
}