#ifndef GAME_GAMEOBJECT_HPP
#define GAME_GAMEOBJECT_HPP

#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/MappedBuffer.hpp>

namespace game::core::components {
    struct GameObject {
        u64 vertex_buffer_idx{};
        u64 vertex_count{};

        api::DescriptorSet descriptor_set{};
        api::MappedBuffer instance_buffer{};
    };
} // namespace game::core::components

#endif //GAME_GAMEOBJECT_HPP
