module;
#include <functional>
#include <numeric>
#include <optional>
#include <chrono>
#include <tuple>
#include <bitset>
#include <atomic>

#define GLM_ALIGNED_TYPEDEF
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "glm/gtc/matrix_transform.hpp"
export module Animation;
export import obscure.utils.stopwatch;

export using duration_t = obscure::stopwatch<float>::duration_t;

export template<typename T>
concept animation_c =
        requires(T t, duration_t time)
        {
            { t.should_evaluate(time) } noexcept -> std::convertible_to<bool>;
        }
        && requires(T t, glm::mat4 m, duration_t time, duration_t dt)
        {
            { t.evaluate(m, time, dt) } noexcept -> std::convertible_to<glm::mat4>;
        };


template<animation_c ... Animations>
struct eval_animation_impl;

template<animation_c TAnimation1, animation_c ... TAnimations>
struct eval_animation_impl<TAnimation1, TAnimations...> {

    static bool should_evaluate(duration_t time, TAnimation1 & anim1, TAnimations & ... anims) noexcept {
        if (anim1.should_evaluate(time)) {
            return true;
        }
        else {
            return eval_animation_impl<TAnimations...>::should_evaluate(time, anims...);
        }
    }

    static glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time, TAnimation1 & anim1, TAnimations & ... rest) noexcept {
        if (anim1.should_evaluate(time)) {
            current = anim1.evaluate(current, time, frame_time);
        }

        return eval_animation_impl<TAnimations...>::evaluate(current, time, frame_time, rest...);
    }
};

template<>
struct eval_animation_impl<> {

    static constexpr bool should_evaluate(duration_t time) noexcept {
        return false;
    }

    static constexpr glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        return current;
    }
};

export template<animation_c ... animation_t>
glm::mat4 evaluate_animations(glm::mat4 current, duration_t time, duration_t frame_time, animation_t &... animations) {
    return eval_animation_impl<animation_t...>::evaluate(current, time, frame_time, animations...);
}

export template<animation_c ... animation_t>
glm::mat4 evaluate_animations(glm::mat4 current, animation_t &... animations) {
    static obscure::stopwatch<float> animation_timer{};
    return evaluate_animations<animation_t...>(current, animation_timer.total_time(), animation_timer.lap_time(), animations...);
}

export template<animation_c ... TAnimations>
struct Combine {
    static constexpr bool is_combination = true;
    std::tuple<TAnimations...> animations;

    constexpr Combine(TAnimations... animations) : animations(std::make_tuple(animations...)) {}

    template<animation_c ... TAnimations2>
    static constexpr Combine<TAnimations..., TAnimations2...> combine(Combine a, Combine<TAnimations2...> b) noexcept {
        return std::apply([b](TAnimations ... anims) {
            return std::apply([anims...](TAnimations2 ... anims2) {
                Combine<TAnimations..., TAnimations2...>{ anims..., anims2... };
            }, b.animations);
        }, a.animations);
    }

    bool should_evaluate(duration_t time) noexcept {
        return std::apply([time](TAnimations & ... anims) {
            return eval_animation_impl<TAnimations...>::should_evaluate(time, anims...);
        }, animations);
    }

    glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        return std::apply([current, time, frame_time](TAnimations & ... anims) {
            return evaluate_animations(current, time, frame_time, anims...);
        }, animations);
    }
};

export template<animation_c TAnimation>
consteval bool is_combination() {
    if constexpr (requires { TAnimation::is_combination; }) {
        return TAnimation::is_combination;
    }
    else {
        return false;
    }
}

export template<animation_c TA1, animation_c TA2>
constexpr Combine<TA1, TA2> operator&(TA1 a, TA2 b) noexcept {
    return Combine<TA1, TA2>{std::move(a), std::move(b)};
}

export template<animation_c TA1, animation_c ... TAnimations>
constexpr Combine<TA1, TAnimations...> operator&(TA1 a, Combine<TAnimations...> b) noexcept {
    return std::apply([a](TAnimations... anims) {
        return Combine<TA1, TAnimations...>{std::move(a), std::move(anims)...};
    }, b.animations);
}

export template<animation_c TA1, animation_c ... TAnimations>
    requires (is_combination<TA1>())
constexpr Combine<TA1, TAnimations...> operator&(TA1 a, Combine<TAnimations...> b) noexcept {
    return TA1::combine(std::move(a), std::move(b));
}

export struct Move {
    glm::vec3 velocity;
    constexpr bool should_evaluate(duration_t time) const noexcept {
        return true;
    }

    constexpr glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) const noexcept {
        return glm::translate(current, frame_time.count() * velocity);
    }

};

static_assert(animation_c<Move>);

static_assert(is_combination<Combine<Move, Move>>());

static_assert(!is_combination<Move>());


export struct Rotate {
    glm::vec3 axis;
    float angular_velocity;

    explicit constexpr Rotate(glm::vec3 angular_velocities) noexcept
        : axis(glm::normalize(angular_velocities)),
          angular_velocity(glm::radians(glm::length(angular_velocities)))
    {}

    constexpr bool should_evaluate(duration_t time) const noexcept {
        return true;
    }

    constexpr glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) const noexcept {
        return glm::rotate(current, frame_time.count() * angular_velocity, axis);
    }
};

export struct Identity {
    constexpr bool should_evaluate(duration_t time) const noexcept {
        return true;
    }

    constexpr glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) const noexcept {
        return current;
    }
};

export template<animation_c TAnimation>
struct Duration {
    TAnimation animation;
    duration_t duration;

    bool should_evaluate(duration_t time) noexcept {
        return (time <= duration) && animation.should_evaluate(time);
    }

    glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        return animation.evaluate(current, time, frame_time);
    }
};

export template<animation_c TAnimation>
Duration<TAnimation> operator&(TAnimation anim, duration_t time) noexcept {
    return Duration<TAnimation>{anim, time};
}

export template<animation_c TAnimation>
Duration<TAnimation> operator&(duration_t time, TAnimation anim) noexcept {
    return Duration<TAnimation>{anim, time};
}



template<animation_c ... TAnimations>
struct sequence_impl;

template<animation_c TA1, animation_c ... TAnimations>
struct sequence_impl<TA1, TAnimations...> {
    static bool should_evaluate(duration_t time, Duration<TA1> & d1, Duration<TAnimations> & ... rest) noexcept {
        if (d1.duration >= time) {
            return d1.should_evaluate(time);
        }
        else {
            return sequence_impl<TAnimations...>::should_evaluate(time - d1.duration, rest...);
        }
    }

    static glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time, Duration<TA1> & d1, Duration<TAnimations> & ... rest) noexcept {
        if (d1.duration >= time) {
            return d1.evaluate(current, time, frame_time);
        }
        else {
            return sequence_impl<TAnimations...>::evaluate(current, time - d1.duration, frame_time, rest...);
        }
    }

    static duration_t total_duration(Duration<TA1> d1, Duration<TAnimations> ... rest) noexcept {
        return d1.duration + sequence_impl<TAnimations...>::total_duration(rest...);
    }

};

template<>
struct sequence_impl<> {
    static constexpr bool should_evaluate(duration_t time) noexcept {
        return false;
    }

    static constexpr glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        return current;
    }

    static constexpr duration_t total_duration() noexcept {
        return duration_t{ 0 };
    }
};

export template<animation_c ... TAnimations>
struct Sequence {
    static constexpr bool is_sequence = true;
    std::tuple<Duration<TAnimations>...> animations;

    Sequence(Duration<TAnimations> const& ... durations) noexcept
        : animations(std::make_tuple(durations...))
    {}

    bool should_evaluate(duration_t time) noexcept {
        return std::apply([time](Duration<TAnimations> & ... anims) {
            return sequence_impl<TAnimations...>::should_evaluate(time, anims...);
        }, animations);
    }

    glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        return std::apply([current, time, frame_time](Duration<TAnimations> & ... anims) {
            return sequence_impl<TAnimations...>::evaluate(current, time, frame_time, anims...);
        }, animations);
    }

    template<animation_c ... TAnimations2>
    static constexpr Sequence<TAnimations..., TAnimations2...> combine(Sequence const& a, Sequence<TAnimations2...> const& b) noexcept {
        return std::apply([&b](Duration<TAnimations> const& ... anims) {
            return std::apply([&anims...](Duration<TAnimations2> const& ... anims2) {
                return Sequence<TAnimations..., TAnimations2...>{ anims..., anims2... };
            }, b.animations);
        }, a.animations);
    }
};

export template<animation_c TAnimation>
consteval bool is_sequence() {
    if constexpr (requires { TAnimation::is_sequence; }) {
        return TAnimation::is_sequence;
    }
    else {
        return false;
    }
}

export template<animation_c TA1, animation_c TA2>
Sequence<TA1, TA2> operator|(Duration<TA1> a1, Duration<TA2> a2) noexcept {
    return Sequence<TA1, TA2>{a1, a2};
}

export template<animation_c TA1, animation_c ... TA2>
Sequence<TA1, TA2...> operator|(Duration<TA1> a1, Sequence<TA2...> a2) noexcept {
    return std::apply([a1](Duration<TA2> ... a2) {
        return Sequence<TA1, TA2...>{a1, a2...};
    }, a2);
}

export template<animation_c TA1, animation_c ... TA2>
    requires (is_sequence<TA1>())
auto operator|(TA1 a1, Sequence<TA2...> a2) noexcept {
    return TA1::combine(a1, a2);
}

export template<animation_c ... TAnimations>
struct Loop {
    std::tuple<Duration<TAnimations>...> animations;
    glm::mat4 reset;
    duration_t offset;
    bool should_reset = true;

    explicit Loop(glm::mat4 reset_, Duration<TAnimations>... durations) noexcept
        : animations(std::make_tuple(durations...)),
          reset(reset_),
          offset(0)
    {}

    explicit Loop(glm::mat4 reset_, Sequence<TAnimations...> sequence) noexcept
    : animations(sequence.animations),
       reset(reset_),
       offset(0)
    {}

    [[nodiscard]] duration_t total_duration() const noexcept {
        return std::apply(sequence_impl<TAnimations...>::total_duration, animations);
    }

    bool should_evaluate(duration_t time) noexcept {
        if (offset + total_duration() < time) {
            should_reset = true;
            offset += total_duration();
        }
        time -= offset;
        return std::apply([time](Duration<TAnimations> & ... anims) {
            return sequence_impl<TAnimations...>::should_evaluate(time, anims...);
        }, animations);
    }

    glm::mat4 evaluate(glm::mat4 current, duration_t time, duration_t frame_time) noexcept {
        if (should_reset) {
            current = reset;
            should_reset = false;
        }
        time -= offset;
        return std::apply([current, time, frame_time](Duration<TAnimations> & ... anims) {
            return sequence_impl<TAnimations...>::evaluate(current, time, frame_time, anims...);
        }, animations);
    }


};

static_assert(animation_c<Move>);