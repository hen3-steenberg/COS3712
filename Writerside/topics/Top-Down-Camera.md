# Top Down Camera

In this mode the orientation of the camera is locked such that there is only 4-DOF.
 1. Move Forward and Backwards.
 2. Move Left and Right.
 3. Move Up and Down.
 4. Rotate Left and Right.

```C++
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
```
From `src/Camera.cppm` Lines 105-121.