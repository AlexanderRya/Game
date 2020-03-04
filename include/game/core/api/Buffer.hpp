#ifndef GAME_BUFFER_HPP
#define GAME_BUFFER_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

namespace game::core::api {
    struct Buffer {
        vk::Buffer handle{};
        VmaAllocation allocation{};
    };

    [[nodiscard]] Buffer vma_make_buffer(const usize, const vk::BufferUsageFlags&, const VmaMemoryUsage, const VmaAllocationCreateFlags, const VulkanContext&);
    [[nodiscard]] vk::Buffer make_buffer(const usize, const vk::BufferUsageFlags&, const VulkanContext&);
    [[nodiscard]] vk::DeviceMemory allocate_memory(const vk::Buffer&, const vk::MemoryPropertyFlags&, const VulkanContext&);
    void copy_buffer(const vk::Buffer&, const vk::Buffer&, const usize, const VulkanContext&);
} // namespace game::core::api

#endif //GAME_BUFFER_HPP
