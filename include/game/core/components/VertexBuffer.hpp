#ifndef GAME_VERTEXBUFFER_HPP
#define GAME_VERTEXBUFFER_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

namespace game::core::components {
    struct VertexBuffer {
        vk::Buffer handle{};
        vk::DeviceMemory memory{};
    };

    [[nodiscard]] VertexBuffer make_vertex_buffer(const std::vector<Vertex>&, const api::VulkanContext&);
} // namespace game::core::components

#endif //GAME_VERTEXBUFFER_HPP
