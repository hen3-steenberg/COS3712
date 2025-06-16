# Documentation

## Overview
The project itself consists of a few directories, each with a specific purpose.

### 1. `src`
The `src` directory contains all of the source files that is compiled into the final program.
This includes resources in their final states.

Following is a quick overview over the C++ files in this directory.

 1. `cos3712.cpp`
This is the entry point and where everything is brought together.
 2. `Resources.cppm` + `resources/Resources.cxx`
This is the module in which the resources are embedded at compilation time. 
It provides functionality to access these resources.
 3. `GlobalState.cppm`
This module provides shared access for variables that is needed in more than one other module.
 4. `Floor.cppm`
This module defines the graphics pipeline for drawing the floor.
 5. `BuildingPipe.cppm`
This module defines the graphics pipeline for drawing buildings. 
As well as a definition for building objects.
 6. `Camera.cppm`
This module interprets user input and translates it into the appropriate camera movement.
 7. `MenuOverlay.cppm`
Provides the functions to draw the various menus and overlays and respond to any events that is triggered through the menu.

### 2. `shaders`
The `shaders` directory contains the source code for all of the shaders used in the program.

The code for both the vertex (`.vert`) and fragment (`.frag`) shaders for the following entities can be found in this directory.
 - building
 - floor

The glNoise shader library is located in the `glNoise` subdirectory.

The instructions for compiling the shaders is located here in the `CMakeLists.txt` file.

The shaders are compiled to `src/resources/shaders`.

### 3. `objects`
This directory contains the Blender (`.blend`) files for various objects that needs to be drawn when the program is run.

### 4. `obscure`
This directory contains the source for the obscure library.

### 5. `3rd_party`
The `3rd_party` directory contains the sources for most of the dependencies required by the project.

### 6. `Writerside`
The `Writerside` directory contains the source files for this document.
