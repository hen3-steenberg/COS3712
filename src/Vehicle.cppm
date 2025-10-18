module;
#include <memory>
#include <span>
#include <string_view>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "glm/ext/matrix_transform.hpp"
export module Vehicle;
import ObjectPipe;

export struct Vehicle {
    std::shared_ptr<ObjectModel> model;
    glm::mat4 transform;

    template<typename Ctx>
    Vehicle(Ctx const& ctx, std::span<const char> obj, std::string_view mtl)
        : model(std::make_shared<ObjectModel>(ObjectModel::load_from_memory(ctx, obj, mtl))),
        transform(glm::identity<glm::mat4>())
    {}

    Vehicle(Vehicle const& other)
        : model(other.model),
        transform(glm::identity<glm::mat4>())
    {}

    Vehicle(Vehicle&& other)
        : model(std::move(other.model)),
        transform(other.transform)
    {}
};