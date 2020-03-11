#ifndef GAME_TRANSFORM_HPP
#define GAME_TRANSFORM_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace game::core::components {
    struct alignas(16) ModelColor {
        glm::mat4 model;
        glm::vec3 color;
    };

    struct Transform {
        glm::vec3 position{};
        glm::vec3 size{};
        glm::vec3 color{};
        glm::vec3 velocity{};
        float rotation{};
        bool is_solid{};
        bool is_destroyed{};
    };
} // namespace game::core::components

#endif //GAME_TRANSFORM_HPP
