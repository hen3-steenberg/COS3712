# Buildings

Buildings are represented by an object model and a list of transformations.

It is assumed that the transformation for each building will remain relatively static and as such all of the
transformations for each building shape is stored together (see `src/Building.cppm`).

The buildings are drawn using the `ObjectPipe` pipelines `draw_objects` function.
This function takes in a model along with a list of transforms, sends the model to the GPU once and then redraws that model for each transform in the list (See `src/ObjectPipe.cppm`).

```C++
		frame.draw_objects(viewproj, portal.locations, portal.model);
		frame.draw_objects(viewproj, hole.locations, hole.model);
		frame.draw_objects(viewproj, building1.locations, building1.model);
		frame.draw_objects(viewproj, building2.locations, building2.model);
		frame.draw_objects(viewproj, building3.locations, building3.model);
```
From `src/cos3712.cpp` Lines 204-208.

The model for each building is loaded when the building is initialized.
Locations are later added by calling the function `add_instance`.
`add_instance` takes an angle that the model will be rotated by (around the UP axis) and the location that the building will be drawn at (It is assumed that the building is near the origin of the model).

There are a total of 8 buildings that is drawn in the world.
```C++
		portal.add_instance(225.0f, glm::vec3{50.0f, 50.0f, 0.0f});
		portal.add_instance(90.0f, glm::vec3{-70.0f, -100.0f, 5.0f});

		hole.add_instance(0.0f, glm::vec3{75.0f, -38.2f, 0.0f});
		hole.add_instance(0.0f, glm::vec3{-75.0f, 38.3f, 0.0f});

		building2.add_instance(0.0f, glm::vec3{-10.0f, 30.0f, 5.0f});
		building2.add_instance(180.0f, glm::vec3{10.0f, 40.0f, 1.0f});

		building1.add_instance(0.0f, glm::vec3{-50.0f, 0.0f, 0.0f});



		building3.add_instance(0.0f, glm::vec3{50.0f, 0.0f, 2.0f});
```
From `src/cos3712.cpp` Lines 130-143.