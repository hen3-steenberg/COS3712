# obscure

Obscure is my rendering engine which I wrote.

The main aim or differentiator of this rendering engine is that it assumes that the user wants to customize the
pipelines that the application will use and tries to make it very easy to define custom pipelines.

To define a custom pipeline the user has to define a structure with specific members.
## 1. `shader_list` typename
A compile time list of shaders that the pipeline requires.

A template specialization of the struct `shader_loader` is required for each shader in the list.
The `shader_loader` specialization should provide a static method `load_shader()` which returns the data of the compiled shader
as something which can be converted into a `std::span<const uint32_t>` the concept `obscure::vulkan::shader_data` is provided to confirm the return type is valid at compile time.

### Examples
```c++
export struct ObjectPipe {
    using shader_list = obscure::make_set<
        resources::shader_name::object_vertex,
        resources::shader_name::object_fragment
    >;
    ...
    };
```
From `src/ObjectPipe.cppm` lines 68-72.
```c++
export
template<resources::shader_name shader>
struct shader_loader<shader> {
    static obscure::vulkan::shader_data auto load_shader() {
        return resources::get_data<shader>();
    }
};
```
From `src/Resources.cppm` lines 76-82;

## 2. `initialize` function
The `initialize` function handles the runtime initialization of the pipeline.
The `initialize` function should return a value of a type that satisfies the `obscure::vulkan::pipeline_builder` concept.

Obscure provides a function `obscure::vulkan::default_pipeline_builder`, that initializes all of the fixed function configuration options for the pipeline.

```c++
        auto result = obscure::vulkan::default_pipeline_builder<1, 2,
            vk::PrimitiveTopology::eTriangleList,
            vk::PolygonMode::eFill,
            vk::FrontFace::eCounterClockwise,
            vk::ShaderStageFlagBits::eVertex,
            vk::ShaderStageFlagBits::eFragment>
        (
            render_pass,
            samples,
            shaders,
            {
                vk::VertexInputBindingDescription{
                    0,
                    sizeof(ObjectModel::vertex_t),
                    vk::VertexInputRate::eVertex
                }
            },
            {
                vk::VertexInputAttributeDescription{
                    0,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(ObjectModel::vertex_t, position)
                },
                vk::VertexInputAttributeDescription{
                    1,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    offsetof(ObjectModel::vertex_t, normal)
                }
            });
```
From `src/ObjectPipe.cppm` lines 80-110.

Further configuration is then left to the implementer.

## 2. `draw_calls` typename
The `draw_calls` type must inherit the `obscure::vulkan::draw_call_base`.
This struct contains all of the code for invoking the graphics pipeline with various parameters.
Any public functions of the `draw_calls` type is exposed in the `frame_ctx`. The `frame_ctx` is obtained by calling `begin_frame()` on the `graphics_context`.