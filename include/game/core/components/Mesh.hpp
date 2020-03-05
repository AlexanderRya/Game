#ifndef GAME_MESH_HPP
#define GAME_MESH_HPP

#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/MappedBuffer.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vector>

namespace game::core::components {
    struct Mesh {
        struct BuildInfo {
            const api::VulkanContext* ctx{};

            vk::DescriptorSetLayout layout{};
            const api::MappedBuffer* camera_buffer{};

            const Texture* texture{};
        };
        u64 vertex_count{};
        u64 vertex_buffer_id{};
        u64 texture_idx{};

        void (*update)(Mesh&);

        std::vector<Instance> instances{};
        api::DescriptorSet descriptor_set{};
        api::MappedBuffer instance_buffer{};

        void build(const BuildInfo&);
    };

    [[nodiscard]] std::vector<Vertex> generate_triangle_geometry();
    [[nodiscard]] std::vector<Vertex> generate_quad_geometry();
}

#endif //GAME_MESH_HPP
