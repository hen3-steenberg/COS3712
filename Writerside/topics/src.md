# src

Following is a quick overview over the C++ files in this directory.

1. `cos3712.cpp`
   This is the entry point and where everything is brought together.
2. `Animation.cppm`
   A module with all of the programming for different animations and combining them.
3. `Building.cppm`
   The code for tying together building models and the locations where they should be drawn.
4. `Camera.cppm`
   This module interprets user input and translates it into the appropriate camera movement.
5. `Drone.cppm`
   The module for animating and managing the drones.
6. `Floor.cppm`
   This module defines the graphics pipeline for drawing the floor.
7. `GlobalState.cppm`
   This module provides shared access for variables that is needed in more than one other module.
8. `MenuOverlay.cppm`
   Provides the functions to draw the various menus and overlays and respond to any events that is triggered through the menu.
9. `ObjectPipe.cppm`
   The code defining the pipeline for drawing different objects.
   This includes Buildings, Vehicles and drones.
10. `Resources.cppm` + `resources/Resources.cxx`
   This is the module in which the resources are embedded at compilation time.
   It provides functionality to access these resources.
11. `Vehicle.cppm`
   Code for sharing the same model between different different vehicles with each having a distinct transformation.

There is also a `resources` subfolder containing all of the resources that is embedded in the application in their final processed state.
1. `objects`
   Each of the blender objects (`.blend`) are exported in the wavefront format which exports an object (`.obj`) file and a material file (`.mtl`)
2. `shaders`
   Contains all of the shaders that the application use compiled to the SPIR-V (`.spv`) format.
3. `textures`
   Contains all of the textures used by the application, currently empty.

