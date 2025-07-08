# COS3712
This uses a rendering engine based on Vulkan that I wrote myself. Called obscure (Because I had no idea what I was doing).

## 1. Dependencies
To make the build process more repeatable most dependencies are included in the project as submodules (The files are in this project).
There are two major exceptions.
### 1.1 A c++ compiler
More specifically the project uses the C23 `#embed` feature to embed resources such as shaders / objects / textures into the executable.
At the time of writing both the `clang` and `gcc` toolchains support this feature.

Since the `clang` compiler is available on all major desktop platforms the project is configured to use the clang compiler if it is detected (`CMakePresets.json`).

To see if other compilers support this feature, see the [C23 Compiler support](https://en.cppreference.com/w/c/compiler_support/23) website.

### 1.2 Vulkan SDK
The vulkan sdk is just too large to include in the project and needs to be installed manually.

If you use a package manager, the sdk should be available on most and could be installed this way.

Otherwise, go to the [Vulkan SDK Download](https://vulkan.lunarg.com/sdk/home) page and download the installer for your operating system.


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
