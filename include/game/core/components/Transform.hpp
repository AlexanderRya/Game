#ifndef GAME_TRANSFORM_HPP
#define GAME_TRANSFORM_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>

namespace game::core::components {
    struct alignas(16) ModelColor {
        glm::mat4 model;
        glm::vec3 color;
    };

    struct Transform {
        struct Instance {
            glm::vec3 position{};
            glm::vec3 size{};
            glm::vec3 color{};
            float rotation{};
        };

        std::vector<Instance> instances;
    };
} // namespace game::core::components

#endif //GAME_TRANSFORM_HPP
