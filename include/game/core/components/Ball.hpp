#ifndef GAME_BALL_HPP
#define GAME_BALL_HPP

#include <glm/vec3.hpp>

#include <vector>

namespace game::core::components {
    struct Ball {
        glm::vec3 velocity;
        float radius;
        bool is_stuck;
    };
} // namespace game::core::components

#endif //GAME_BALL_HPP
