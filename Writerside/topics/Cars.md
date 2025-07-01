# Cars

Or vehicles.

A vehicle is represented as a shared object model along with a transform.
The object is shared to prevent the need for keeping more than one copy of the same object model in memory.
Each vehicle has their own transform, because the animation for even similar models can be very different and
different vehicles may be using the same model from completely different scopes.

There is currently a single model for a vehicle (`objects/ship.blend`).

There are 5 vehicles drawn and animated in the application.
```C++
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
```
From `src/cos3712.cpp` Lines 97-106.

```C++
		if (global::AnimateCars()) {
			ship1.transform = evaluate_animations(ship1.transform, total_time - vehicle_stop_time, frame_time, ship1_animation);
			ship2.transform = evaluate_animations(ship2.transform, total_time - vehicle_stop_time, frame_time, ship2_animation);
			ship3.transform = evaluate_animations(ship3.transform, total_time - vehicle_stop_time, frame_time, ship3_animation);
			ship4.transform = evaluate_animations(ship4.transform, total_time - vehicle_stop_time, frame_time, ship4_animation);
			ship5.transform = evaluate_animations(ship5.transform, total_time - vehicle_stop_time, frame_time, ship5_animation);
		}
		else {
			vehicle_stop_time += frame_time;
		}

		frame.draw_objects(viewproj,
			{
				ship1.transform,
				ship2.transform,
				ship3.transform,
				ship4.transform,
				ship5.transform
			}, *ship1.model);
```
From `src/cos3712.cpp` Lines 180-198.

It should be noted that `vehicle_stop_time` increases as long as the vehicle animations is paused and is subtracted from the total time.
If this is not done the implementation of loops breaks for a single loop as the wrong sequence of animations is executed.