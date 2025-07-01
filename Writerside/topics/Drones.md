# Drones

Drones are represented similarly to buildings with a model and a list of transforms.
This is because all of the drones has the same animation (a rotation).

There is one drone model `objects/drone.blend`.
```C++
	DroneList drones;
```
From `src/cos3712.cpp` Line 107.

It is drawn 4 times.
```C++
		drones.add_drone(glm::vec3{20.0f, 20.0f, 100.0f});
		drones.add_drone(glm::vec3{-20.0f, -20.0f, 100.0f});
		drones.add_drone(glm::vec3{-20.0f, 20.0f, 100.0f});
		drones.add_drone(glm::vec3{20.0f, -20.0f, 100.0f});
```
From `src/cos3712.cpp` Line 146-149.

The animations will skip the first 2 drones in the list when the option is toggled.
```C++
    void animate() {
        auto update = evaluate_animations(glm::identity<glm::mat4>(), rotation);
        if (global::AnimateFirst2Drones()) {
            for (auto & transform : locations) {
                transform *= update;
            }
        }
        else {
            for (auto & transform : locations | std::ranges::views::drop(2)) {
                transform *= update;
            }
        }
    }
```