module;
#include <vector>
#include <span>
#include <string_view>
#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
export module Building;
import ObjectPipe;

export struct Building {
    ObjectModel model;
    std::vector<glm::mat4> locations;

    template<typename Ctx>
    Building(Ctx const& ctx, std::span<const char> obj, std::string_view mtl)
        : model(ObjectModel::load_from_memory(ctx, obj, mtl)),
        locations()
    {}

    void add_instance(float angle, glm::vec3 offset) {
        glm::mat4 transform = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), glm::vec3{0.0f, 0.0f, 1.0f});
        locations.emplace_back(glm::translate(transform, offset));
    }
};