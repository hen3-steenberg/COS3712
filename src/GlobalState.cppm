module;
#include <GLFW/glfw3.h>
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
}