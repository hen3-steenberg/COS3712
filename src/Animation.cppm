module;
#include <functional>
#include <numeric>
#include <optional>
#include <chrono>

#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "glm/gtc/matrix_transform.hpp"
export module Animation;
export import obscure.utils.stopwatch;

export using duration_t = obscure::stopwatch<float>::duration_t;

export struct Animation {


    using animation_f = std::function<glm::mat4 (glm::mat4, duration_t, duration_t)>;
    static inline obscure::stopwatch<float> animation_timer{};

    duration_t duration;
    animation_f animation_function;
    duration_t wait_time;

    Animation(animation_f func, duration_t duration_)
        : duration(duration),
        animation_function(func),
        wait_time(std::numeric_limits<float>::infinity())
    {}

    bool is_active(duration_t elapsed) {
        if (wait_time.count() == std::numeric_limits<float>::infinity()) {
            wait_time = elapsed;
        }
        if (elapsed < wait_time) {
            return false;
        }
        duration_t my_elapsed = elapsed - wait_time;
        if (my_elapsed >= duration) {
            wait_time = duration_t{std::numeric_limits<float>::infinity()};
            return false;
        }

        return true;
    }

    glm::mat4 operator()(glm::mat4 current, duration_t elapsed, duration_t frame_time) {
        duration_t my_elapsed = elapsed - wait_time;
        return animation_function(current, my_elapsed, frame_time);
    }

    glm::mat4 operator()(glm::mat4 current) {
        return (*this)(current, animation_timer.total_time(), animation_timer.lap_time());
    }
};

export Animation translate(glm::vec3 offset, duration_t duration) {
    glm::vec3 velocity = offset / duration.count();
    auto func = [velocity](glm::mat4 current, duration_t elapsed, duration_t frame_time) -> glm::mat4 {
        return glm::translate(current, frame_time.count() * velocity);
    };
    return Animation{
        func,
        duration,
    };
}