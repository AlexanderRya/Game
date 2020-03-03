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
        u64 vertex_count{};
        u64 vertex_buffer_id{};

        std::vector<Instance> instance_data{};
        api::DescriptorSet descriptor_set{};
        api::MappedBuffer instance_buffer{};

        void build(const api::VulkanContext&, const api::PipelineLayout&);
    };

    [[nodiscard]] std::vector<Vertex> generate_triangle_geometry();
}

#endif //GAME_MESH_HPP
