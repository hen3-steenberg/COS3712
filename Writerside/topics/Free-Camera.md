# Free Camera

The free camera is the oposite of the Top Down Camera where the only restriction is that the camera will always remain "upright".

```C++
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
```
From `src/Camera.cppm` Lines 