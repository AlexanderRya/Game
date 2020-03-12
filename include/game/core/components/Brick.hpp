#ifndef GAME_BRICK_HPP
#define GAME_BRICK_HPP

#include <game/core/components/Transform.hpp>
#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/MappedBuffer.hpp>

#include <glm/vec3.hpp>

#include <vector>

namespace game::core::components {
    struct Brick {
        struct Info {
            bool is_solid{};
            bool is_destroyed{};
        };

        std::vector<Info> info{};
    };
} // namespace game::core::components

#endif //GAME_BRICK_HPP
