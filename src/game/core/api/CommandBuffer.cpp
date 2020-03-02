#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/Logger.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

namespace game::core::api {
    std::vector<vk::CommandBuffer> make_rendering_command_buffers(const VulkanContext& ctx) {
        vk::CommandBufferAllocateInfo allocate_info{}; {
            allocate_info.commandPool = ctx.command_pool;
            allocate_info.commandBufferCount = ctx.swapchain.image_count;
            allocate_info.level = vk::CommandBufferLevel::ePrimary;
        }

        auto buffers = ctx.device.logical.allocateCommandBuffers(allocate_info, ctx.dispatcher);

        logger::info("Created ", allocate_info.commandBufferCount, " command buffers for rendering");

        return buffers;
    }
} // namespace game::core::api