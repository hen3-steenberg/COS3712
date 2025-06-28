module;
#include <functional>
#include <GLFW/glfw3.h>
#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
export module GlobalState;
export import obscure.glfw;

export namespace global
{
    enum class CameraMode {
        TopDown,
        Free
    };

    obscure::glfw::glfw_window_ref & windowRef() {
        static obscure::glfw::glfw_window_ref window;
        return window;
    }

    CameraMode & CurrentCameraMode() {
        static CameraMode camera_mode = CameraMode::TopDown;
        return camera_mode;
    }

    void toggleCameraMode() {

        constexpr static int normal_cursor = GLFW_CURSOR_NORMAL;
        constexpr static int no_cursor = GLFW_CURSOR_DISABLED;
        int cursorMode = global::CurrentCameraMode() == global::CameraMode::Free ? normal_cursor : no_cursor;
        glfwSetInputMode(windowRef().window_ptr, GLFW_CURSOR, cursorMode);
        if (CurrentCameraMode() == CameraMode::TopDown) {
            CurrentCameraMode() = CameraMode::Free;
        }
        else {
            CurrentCameraMode() = CameraMode::TopDown;
        }
    }

    glm::vec3 & cameraPosition() {
        static glm::vec3 camera_position {0.0f, 0.0f, 200.0f};
        return camera_position;
    }

    template<int KEY>
    void toggleKey(std::function<void()> toggle_action) {
        //debounce flag
        static bool is_key_released = true;
        if (windowRef().isKeyPressed(KEY)) {
            if (is_key_released) {
                // toggle callback
                toggle_action();
                is_key_released = false;
            }
        }
        else {
            //debounce once key is released.
            is_key_released = true;
        }
    }

    bool & AnimateFirst2Drones() {
        static bool animate_first_2_dones = true;

        toggleKey<GLFW_KEY_R>([]() {
            animate_first_2_dones = !animate_first_2_dones;
        });

        return animate_first_2_dones;
    }

    bool & AnimateCars() {
        static bool animate_cars = true;
        toggleKey<GLFW_KEY_T>([]() {
            animate_cars = !animate_cars;
        });
        return animate_cars;
    }
}