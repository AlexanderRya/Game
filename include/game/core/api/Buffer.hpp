#ifndef GAME_BUFFER_HPP
#define GAME_BUFFER_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

namespace game::core::api {
    [[nodiscard]] vk::Buffer make_buffer(const usize, const vk::BufferUsageFlags&, const VulkanContext&);
    [[nodiscard]] vk::DeviceMemory allocate_memory(const vk::Buffer&, const vk::MemoryPropertyFlags&, const VulkanContext&);
    void copy_buffer(const vk::Buffer&, const vk::Buffer&, const usize, const VulkanContext&);
} // namespace game::core::api

#endif //GAME_BUFFER_HPP
