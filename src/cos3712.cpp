#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <print>
#include <vulkan/vulkan.hpp>

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

constexpr auto
build_ship1_animation()
{
    auto sequence = (Rotate{ glm::vec3{ 0.0f, 0.0f, 15.0f } } & 12s) | (Identity{} & 15s);
    return Move{ glm::vec3{ 10.0f, 0.0f, 0.0f } } &
           Loop{ glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 75.0f, -38.2f, 30.0f }), sequence | sequence };
}

constexpr auto
build_ship2_animation()
{
    glm::mat4 start = glm::rotate(glm::translate(glm::identity<glm::mat4>(), glm::vec3{ -75.0f, 38.2f, 30.0f }),
                                  glm::radians(180.0f),
                                  glm::vec3{ 0.0f, 0.0f, 1.0f });
    auto sequence = (Rotate{ glm::vec3{ 0.0f, 0.0f, 15.0f } } & 12s) | (Identity{} & 15s);
    return Move{ glm::vec3{ 10.0f, 0.0f, 0.0f } } & Loop{ start, sequence | sequence };
}

constexpr auto
build_ship3_animation()
{
    glm::mat4 start = glm::translate(glm::identity<glm::mat4>(), glm::vec3{ -75.0f, -38.2f, 30.0f });
    auto sequence = (Identity{} & 15s) | (Rotate{ glm::vec3{ 0.0f, 0.0f, 15.0f } } & 12s);
    return Move{ glm::vec3{ 10.0f, 0.0f, 0.0f } } & Loop{ start, sequence | sequence };
}

constexpr auto
build_ship4_animation()
{
    glm::mat4 start = glm::rotate(glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 75.0f, 38.2f, 30.0f }),
                                  glm::radians(180.0f),
                                  glm::vec3{ 0.0f, 0.0f, 1.0f });
    auto sequence = (Identity{} & 15s) | (Rotate{ glm::vec3{ 0.0f, 0.0f, 15.0f } } & 12s);
    return Move{ glm::vec3{ 10.0f, 0.0f, 0.0f } } & Loop{ start, sequence | sequence };
}

constexpr auto
build_ship5_animation()
{
    glm::mat4 start = glm::rotate(glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 50.0f, 50.2f, 3.0f }),
                                  glm::radians(225.0f),
                                  glm::vec3{ 0.0f, 0.0f, 1.0f });
    auto sequence = (Identity{} & 1s) | (Move{ glm::vec3{ 0.0f, 0.0f, 2.2f } } & 2s) | (Identity{} & 12400ms) |
                    (Rotate{ glm::vec3{ 0.0f, 0.0f, 15.0f } } & 3s) | (Identity{} & 1400ms);
    return Move{ glm::vec3{ 10.0f, 0.0f, 0.0f } } & Loop{ start, sequence };
}

struct app_t {
    using gfx_ctx_t = obscure::graphics_context<ObjectPipe, Floor>;
    using frame_t = gfx_ctx_t::command_session_t;
    using ship1_animation_t = decltype(build_ship1_animation());
    using ship2_animation_t = decltype(build_ship2_animation());
    using ship3_animation_t = decltype(build_ship3_animation());
    using ship4_animation_t = decltype(build_ship4_animation());
    using ship5_animation_t = decltype(build_ship5_animation());

    gfx_ctx_t gfx_ctx;
    obscure::imgui::ctx imgui_ctx;
    obscure::vulkan::rgba_2d_texture<> color_texture_floor;
    Building portal;
    Building hole;
    Building building1;
    Building building2;
    Building building3;
    Vehicle ship1;
    ship1_animation_t ship1_animation;
    Vehicle ship2;
    ship2_animation_t ship2_animation;
    Vehicle ship3;
    ship3_animation_t ship3_animation;
    Vehicle ship4;
    ship4_animation_t ship4_animation;
    Vehicle ship5;
    ship5_animation_t ship5_animation;
    DroneList drones;

    app_t()
        : gfx_ctx()
        , imgui_ctx(gfx_ctx)
        , color_texture_floor(gfx_ctx.load_texture<Floor>(resources::floor_color_png(), 0))
        , portal(gfx_ctx, resources::portal_obj(), ""sv)
        , // resources::portal_mtl()),
        hole(gfx_ctx, resources::hole_building_obj(), ""sv)
        , // resources::hole_building_mtl()),
        building1(gfx_ctx, resources::building1_obj(), ""sv)
        , // resources::building1_mtl()),
        building2(gfx_ctx, resources::building2_obj(), ""sv)
        , // resources::building2_mtl()),
        building3(gfx_ctx, resources::building3_obj(), ""sv)
        , // resources::building3_mtl()),
        ship1(gfx_ctx, resources::ship_obj(), ""sv)
        , // resources::ship_mtl()),
        ship1_animation(build_ship1_animation())
        , ship2(ship1)
        , ship2_animation(build_ship2_animation())
        , ship3(ship1)
        , ship3_animation(build_ship3_animation())
        , ship4(ship1)
        , ship4_animation(build_ship4_animation())
        , ship5(ship1)
        , ship5_animation(build_ship5_animation())
        , drones(gfx_ctx)
    {
        portal.add_instance(225.0f, glm::vec3{ 50.0f, 50.0f, 0.0f });
        portal.add_instance(90.0f, glm::vec3{ -70.0f, -100.0f, 5.0f });

        hole.add_instance(0.0f, glm::vec3{ 75.0f, -38.2f, 0.0f });
        hole.add_instance(0.0f, glm::vec3{ -75.0f, 38.3f, 0.0f });

        building2.add_instance(0.0f, glm::vec3{ -10.0f, 30.0f, 5.0f });
        building2.add_instance(180.0f, glm::vec3{ 10.0f, 40.0f, 1.0f });

        building1.add_instance(0.0f, glm::vec3{ -50.0f, 0.0f, 0.0f });

        building3.add_instance(0.0f, glm::vec3{ 50.0f, 0.0f, 2.0f });

        drones.add_drone(glm::vec3{ 20.0f, 20.0f, 100.0f });
        drones.add_drone(glm::vec3{ -20.0f, -20.0f, 100.0f });
        drones.add_drone(glm::vec3{ -20.0f, 20.0f, 100.0f });
        drones.add_drone(glm::vec3{ 20.0f, -20.0f, 100.0f });

        global::windowRef() = gfx_ctx.window.get_window_ref();
    }

    void
    loop(frame_t frame)
    {

        static obscure::stopwatch<float> timer{};
        auto frame_time = timer.lap_time();
        auto total_time = timer.total_time();
        static obscure::stopwatch<float>::duration_t vehicle_stop_time{ 0.0f };
#pragma region perspective
        auto extent = frame.get_extent();

        glm::mat4 proj = glm::perspective(glm::radians(60.0f), extent.width / (float)extent.height, 0.1f, 1000.0f);
        proj[1][1] *= -1;

        glm::mat4 viewproj = proj * GetCameraTransform();
#pragma endregion

#pragma region drones

        drones.animate();

        frame.draw_objects(viewproj, drones.locations, drones.model);

#pragma endregion

#pragma region vehicles

        if (global::AnimateCars()) {
            ship1.transform =
                evaluate_animations(ship1.transform, total_time - vehicle_stop_time, frame_time, ship1_animation);
            ship2.transform =
                evaluate_animations(ship2.transform, total_time - vehicle_stop_time, frame_time, ship2_animation);
            ship3.transform =
                evaluate_animations(ship3.transform, total_time - vehicle_stop_time, frame_time, ship3_animation);
            ship4.transform =
                evaluate_animations(ship4.transform, total_time - vehicle_stop_time, frame_time, ship4_animation);
            ship5.transform =
                evaluate_animations(ship5.transform, total_time - vehicle_stop_time, frame_time, ship5_animation);
        } else {
            vehicle_stop_time += frame_time;
        }

        frame.draw_objects(viewproj,
                           { ship1.transform, ship2.transform, ship3.transform, ship4.transform, ship5.transform },
                           *ship1.model);

#pragma endregion

#pragma region buildings

        frame.draw_objects(viewproj, portal.locations, portal.model);
        frame.draw_objects(viewproj, hole.locations, hole.model);
        frame.draw_objects(viewproj, building1.locations, building1.model);
        frame.draw_objects(viewproj, building2.locations, building2.model);
        frame.draw_objects(viewproj, building3.locations, building3.model);

#pragma endregion

#pragma region sun
        if (global::updateSun()) {
            global::sunAngle() += frame_time.count() * global::sunSpeed();
        }
#pragma endregion

#pragma region floor

        frame.draw_floor(viewproj, color_texture_floor);

#pragma endregion

#pragma region overlay

        drawMenuOverlay(frame.get_command_buffer());

#pragma endregion
    }

    void
    run()
    {
        while (!gfx_ctx.window.should_close()) {
            glfwPollEvents();
            if (gfx_ctx.window.isKeyPressed(GLFW_KEY_ESCAPE) || MenuExit()) {
                break;
            }
            loop(gfx_ctx.begin_frame());
            gfx_ctx.submit_frame();
            gfx_ctx.draw_frame();
        }
    }
};

int
main()
{
    std::println("The size of the CPU resources is {} bytes.", sizeof(app_t));
    obscure::initialize("Test App", obscure::version{ 1, 0, 0 });
    {
        app_t app{};
        app.run();
    }
    obscure::free_instance();

    return 0;
}