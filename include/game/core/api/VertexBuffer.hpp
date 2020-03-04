#ifndef GAME_VERTEXBUFFER_HPP
#define GAME_VERTEXBUFFER_HPP

#include <game/core/api/Buffer.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

namespace game::core::api {
    struct VertexBuffer {
        Buffer buffer{};
    };

    [[nodiscard]] VertexBuffer make_vertex_buffer(const std::vector<Vertex>&, const api::VulkanContext&);
} // namespace game::core::api

#endif //GAME_VERTEXBUFFER_HPP
