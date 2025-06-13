# COS3712
This uses a rendering engine based on Vulkan that I wrote myself. Called obscure (Because I had no idea what I was doing).

## 1. Dependencies
The project depends on the following
### 1.1 \#embed
This project relies on the c23/c++26 feature `#embed` to include resources such as 
shaders / textures / objects
into the binary application. This removes having to distribute any dependencies allong side the application.
However, not all compilers support this feature yet.

This project can be built by the clang compiler version `19.7` or higher.
The clang compiler is available on Windows/Linux/Apple.

Furthermore, the CMake project is set up to automatically use the clang compiler from an IDE if it is available (`CMakePresets.json`)

Note that this was not tested on an Apple device.

### 1.2 Obscure
Obscure is included in the folder `./obscure`

Except for the `#embed` feature, obscure itself relies on the following libraries.

 1. Vulkan
    - Graphics API
    - Needs to be installed manually
 2. GLFW
    - Portable window and input API
    - Needs to be installed manually
 3. GLM
    - C/C++ type definitions for glsl builtin types.
    - Needs to be installed manually
 4. VulkanMemoryAllocator
    - A gpu memory allocator for various vulkan objects.
    - Needs to be installed manually
 5. ImGui
    - A library for rendering controls and overlays over other things.
    - Is included in `./obscure/3rd_party/imgui`

The libraries that is needs to be installed manually is searched for by CMakes `find_package` method.

Practically this means that the dependencies can be installed system-wide on linux, 
if that is not preferred or when working on windows, these packages can be installed with `vcpkg`.

Other C++ package managers can be used as long as the `CMAKE_TOOLCHAIN_FILE` for that package manager is specified manually.

## 2. Build
### 2.1 List the presets for your system
```shell
cmake --list-presets
```
The output will look something like
```text
Available configure presets:

  "linux-debug"   - Linux Debug
  "linux-release" - Linux Release
```
The ID of the preset is the quoted string.
Pick a preset and remember its ID that id will be referenced as `<preset-id>` going forward.
### 2.2 Create a subdirectory
```shell
mkdir build && cd build
```
The following instructions will assume that you are in a subdirectory of the project.
If you want to use a different name for this directory you are welcome.
### 2.3 Generate the build files
```shell
cmake -S .. -B . -G Ninja --preset=<preset>
```
As an example if I wanted to create a debug build on linux the command would be.
```shell
cmake -S .. -B . -G Ninja --preset="linux-debug"
```
If this command executes successfully all dependencies have been found by cmake.


If it is needed to try again the command must be modified a bit.
```shell
cmake -S .. -B . -G Ninja --preset=<preset> --fresh
```
### 2.4 Build the project
```shell
cmake --build ./
```

### 2.5 Run the project
Linux
```shell
./cos3712
```
Windows
```shell
./cos3712.exe
```