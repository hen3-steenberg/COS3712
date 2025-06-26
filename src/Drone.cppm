module;
#include <vector>

#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "glm/gtc/matrix_transform.hpp"
export module Drone;
import ObjectPipe;
import Animation;
import Resources;

export struct DroneList {
    ObjectModel model;
    Rotate rotation;
    std::vector<glm::mat4> locations;

    template<typename CtxT>
    explicit DroneList(CtxT const& ctx)
        : model(ObjectModel::load_from_memory(ctx, resources::drone_obj(), resources::drone_mtl())),
          rotation(glm::vec3{0.0f, 0.0f, 200.0f}),
          locations()
    {}

    void animate() {
        auto update = evaluate_animations(glm::identity<glm::mat4>(), rotation);
        for (auto & transform : locations) {
            transform *= update;
        }
    }

    void add_drone(glm::vec3 offset) {
        locations.emplace_back(glm::translate(glm::identity<glm::mat4>(), offset));
    }
};