#ifndef GAME_GAMEOBJECT_HPP
#define GAME_GAMEOBJECT_HPP

#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/MappedBuffer.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vector>

namespace game::core::components {


    struct GameObject {
        struct BuildInfo {
            const api::VulkanContext* ctx{};

            vk::DescriptorSetLayout layout{};
            const api::MappedBuffer* camera_buffer{};

            const Texture* texture{};
        };
        u64 vertex_count{};
        u64 vertex_buffer_id{};
        u64 texture_idx{};

        api::DescriptorSet descriptor_set{};

        std::vector<GameObjectInfo> info{};

        std::vector<Instance> instances{};
        api::MappedBuffer instance_buffer{};
        std::vector<ColorInstance> colors{};
        api::MappedBuffer color_buffer{};

        void build(const BuildInfo&);
    };

    [[nodiscard]] std::vector<Vertex> generate_triangle_geometry();
    [[nodiscard]] std::vector<Vertex> generate_quad_geometry();
}

#endif //GAME_GAMEOBJECT_HPP
