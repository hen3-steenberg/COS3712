# Dependencies

As mentioned in the [Getting Started](Getting-Started.md) section most of the dependencies are already submodules of the project.
This section explains exactly what those dependencies are and where they are used.

## 1. Obscure
This is the main dependency of the project.
Obscure is a rendering engine that I wrote from scratch. It aims to provide the boilerplate code needed
to use vulkan while making it easy to add custom graphics pipelines.

### 1.1 Vulkan SDK
The vulkan API is central to the project and is the API through which the various graphics pipelines are executed and ultimately displayed.

The vulkan sdk also supports the executable `glslc` which is used to compile all glsl shaders. 
These compiled shaders are easier to interpret by GPU drivers making the graphics pipeline consistent across a wide variety of GPU's.

### 1.2 GLFW
This library manages the output window / screen as well as any mouse / keyboard input.

### 1.3 VulkanMemoryAllocator
In vulkan it is possible to allocate memory on the GPU being used. 
This single memory allocation can then be used by various buffers or textures. 
Memory allocation strategy however, is a very large / specialized topic, 
thus I opted to just use an industry standard library to manage this aspect of vulkan for me.

### 1.4 glm
This library defines C++ types for all of the intrinsic types that is available in the glsl language.
They can be used to do operations on data that will be sent to the GPU and will behave in a similar way.

## 2. rapidobj
The rapidobj library is used to parse Wavefront (`.obj + .mtl`) files which allows the project to use assets generated in Blender
(Blender projects can be exported to the Wavefront format).

## 3. glNoise
glNoise is a glsl library that provides various functions for generating pseudo random noise in shaders.

The floor is generated using perlin noise from this library.

The library is primarily for use in a typescript project, but it is possible to reference the glsl code directly.