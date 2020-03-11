#ifndef GAME_GAMEOBJECT_HPP
#define GAME_GAMEOBJECT_HPP

#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/MappedBuffer.hpp>
#include <game/core/components/Transform.hpp>

#include <vector>

namespace game::core::components {
    struct GameObject {
        u64 vertex_buffer_idx{};
        u64 vertex_count{};

        std::vector<Transform> transforms{};

        api::DescriptorSet descriptor_set{};
        api::MappedBuffer instance_buffer{};
    };
} // namespace game::core::components

#endif //GAME_GAMEOBJECT_HPP
