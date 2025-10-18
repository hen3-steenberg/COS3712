module;
#include <GLFW/glfw3.h>
#include <functional>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <glm/glm.hpp>
export module GlobalState;
export import obscure.glfw;

export namespace global
{
    enum class CameraMode { TopDown, Free };

    obscure::glfw::glfw_window_ref&
    windowRef()
    {
        static obscure::glfw::glfw_window_ref window;
        return window;
    }

    CameraMode&
    CurrentCameraMode()
    {
        static CameraMode camera_mode = CameraMode::TopDown;
        return camera_mode;
    }

    void
    toggleCameraMode()
    {

        constexpr static int normal_cursor = GLFW_CURSOR_NORMAL;
        constexpr static int no_cursor = GLFW_CURSOR_DISABLED;
        int cursorMode = global::CurrentCameraMode() == global::CameraMode::Free ? normal_cursor : no_cursor;
        glfwSetInputMode(windowRef().window_ptr, GLFW_CURSOR, cursorMode);
        if (CurrentCameraMode() == CameraMode::TopDown) {
            CurrentCameraMode() = CameraMode::Free;
        } else {
            CurrentCameraMode() = CameraMode::TopDown;
        }
    }

    glm::vec3&
    cameraPosition()
    {
        static glm::vec3 camera_position{ 0.0f, 0.0f, 200.0f };
        return camera_position;
    }

    template<int KEY>
    void
    toggleKey(std::function<void()> toggle_action)
    {
        // debounce flag
        static bool is_key_released = true;
        if (windowRef().isKeyPressed(KEY)) {
            if (is_key_released) {
                // toggle callback
                toggle_action();
                is_key_released = false;
            }
        } else {
            // debounce once key is released.
            is_key_released = true;
        }
    }

    bool&
    AnimateFirst2Drones()
    {
        static bool animate_first_2_dones = true;

        toggleKey<GLFW_KEY_R>([]() { animate_first_2_dones = !animate_first_2_dones; });

        return animate_first_2_dones;
    }

    bool&
    AnimateCars()
    {
        static bool animate_cars = true;
        toggleKey<GLFW_KEY_T>([]() { animate_cars = !animate_cars; });
        return animate_cars;
    }

    bool&
    updateSun()
    {
        static bool update_sun = true;
        toggleKey<GLFW_KEY_P>([]() { update_sun = !update_sun; });
        return update_sun;
    }

    float&
    sunAngle()
    {
        static float sun_angle = 0.0f;
        if (sun_angle >= 360.0f) {
            sun_angle -= 360.0f;
        }
        return sun_angle;
    }

    float&
    sunSpeed()
    {
        static float sun_speed = 1.0f;
        return sun_speed;
    }

    std::array<float, 3>&
    sunColor()
    {
        static std::array<float, 3> sun_color{ 1.0f, 1.0f, 1.0f };
        return sun_color;
    }

    glm::vec3
    sundirection()
    {
        float radians = sunAngle() * M_PI / 180.0f;
        return glm::vec3{ 0.0f, std::cos(radians), -std::sin(radians) };
    }

    glm::vec3
    getSunColor()
    {
        return glm::vec3{ global::sunColor()[0], global::sunColor()[1], global::sunColor()[2] };
    }

    struct spotLight {
        alignas(16) glm::vec3 direction;
        alignas(16) glm::vec3 position;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
    };
} // namespace global