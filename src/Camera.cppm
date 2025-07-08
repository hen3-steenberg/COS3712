module;
#include <GLFW/glfw3.h>
#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
export module Camera;
import obscure.glfw;
import obscure.utils.stopwatch;
import GlobalState;

void detectCameraMode() {
    global::toggleKey<GLFW_KEY_TAB>(global::toggleCameraMode);
}

glm::vec3 updateCameraPosition(glm::vec3 orientation) noexcept
{
    auto window = global::windowRef();
    glm::vec3 translation = glm::vec3{0.0f, 0.0f, 0.0f};

    if (window.isKeyPressed(GLFW_KEY_W)) {
        translation += orientation;
    }
    if (window.isKeyPressed(GLFW_KEY_S)) {
        translation -= orientation;
    }
    if (window.isKeyPressed(GLFW_KEY_A)) {

        translation -= glm::normalize(glm::cross(orientation, glm::vec3{0.0f, 0.0f, 1.0f}));
    }
    if (window.isKeyPressed(GLFW_KEY_D)) {
        translation += glm::normalize(glm::cross(orientation, glm::vec3{0.0f, 0.0f, 1.0f}));
    }
    if (window.isKeyPressed(GLFW_KEY_SPACE)) {
        translation += glm::vec3{0.0f, 0.0f, 1.0f};
    }
    if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        translation -= glm::vec3{0.0f, 0.0f, 1.0f};
    }
    return (translation != glm::vec3{0.0f, 0.0f, 0.0f})? glm::normalize(translation) : translation;
}

glm::vec3 calculateCameraOrientation(float & pan, float & tilt) noexcept {
    tilt = std::clamp(tilt, -0.99f, 0.99f);
    if (pan < -2.0f) {
        pan += 4.0f;
    }
    if (pan > 2.0f) {
        pan -= 4.0f;
    }

    float pan_angle = pan * glm::half_pi<float>();
    float tilt_angle = tilt * glm::half_pi<float>();

    float pan_cos = glm::cos(pan_angle);
    float pan_sin = glm::sin(pan_angle);
    float tilt_cos = glm::cos(tilt_angle);
    float tilt_sin = glm::sin(tilt_angle);

    return glm::vec3{pan_cos * tilt_cos, pan_sin * tilt_cos, tilt_sin};
}

export glm::mat4 GetCameraTransform() noexcept
{
    static constexpr float camera_speed = 30.0f;
    static glm::vec3 orientation {-1.0f, 0.0f, 0.0f};
    static float pan_angle = 2.0f;
    static float tilt_angle = 0.0f;
    static obscure::stopwatch<float> frame_timer;

    detectCameraMode();
    auto window = global::windowRef();
    float elapsed_seconds = frame_timer.lap_time().count();

    if (global::CurrentCameraMode() == global::CameraMode::Free) {

        global::cameraPosition() += camera_speed * elapsed_seconds * updateCameraPosition(orientation);

#pragma region rotation


        static auto PrevMode = global::CurrentCameraMode();
        static auto [prev_x, prev_y] = window.getCursorPos();
        auto [mouse_x, mouse_y] = window.getCursorPos();
        if (PrevMode == global::CurrentCameraMode()) {
            pan_angle -= elapsed_seconds * static_cast<float>(mouse_x - prev_x);
            tilt_angle -= elapsed_seconds * static_cast<float>(mouse_y - prev_y);

            orientation = calculateCameraOrientation(pan_angle, tilt_angle);

        }
        PrevMode = global::CurrentCameraMode();
        prev_x = mouse_x;
        prev_y = mouse_y;

#pragma endregion
        return glm::lookAt(global::cameraPosition(), global::cameraPosition() + orientation, glm::vec3{0.0f, 0.0f, 1.0f});
    }
    else {


        glm::vec3 up = glm::normalize(glm::vec3{orientation.x, orientation.y, 0.0f});

        global::cameraPosition() += camera_speed * elapsed_seconds * updateCameraPosition(up);

#pragma region rotation

        if (window.isKeyPressed(GLFW_KEY_Q)) {
            pan_angle += elapsed_seconds;
        }
        if (window.isKeyPressed(GLFW_KEY_E)) {
            pan_angle -= elapsed_seconds;
        }
        orientation = calculateCameraOrientation(pan_angle, tilt_angle);

#pragma endregion

        return glm::lookAt(global::cameraPosition(), global::cameraPosition() + glm::vec3{0.0f, 0.0f, -1.0f}, up);
    }




}