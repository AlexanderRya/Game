#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandPool.hpp>
#include <game/Logger.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

namespace snake::core::api {
    vk::CommandPool make_command_pool(const VulkanContext& ctx) {
        vk::CommandPoolCreateInfo command_pool_create_info{}; {
            command_pool_create_info.queueFamilyIndex = ctx.device.queue_family;
            command_pool_create_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        }

        auto pool = ctx.device.logical.createCommandPool(command_pool_create_info, nullptr, ctx.dispatcher);

        logger::info("Command pool successfully created");

        return pool;
    }

    vk::CommandPool make_transient_pool(const VulkanContext& ctx) {
        vk::CommandPoolCreateInfo command_pool_create_info{}; {
            command_pool_create_info.queueFamilyIndex = ctx.device.queue_family;
            command_pool_create_info.flags =
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
                vk::CommandPoolCreateFlagBits::eTransient;
        }

        auto pool = ctx.device.logical.createCommandPool(command_pool_create_info, nullptr, ctx.dispatcher);

        logger::info("Transient command pool successfully created");

        return pool;
    }
} // namespace snake::core::api