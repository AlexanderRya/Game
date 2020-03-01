#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/Framebuffer.hpp>
#include <game/Logger.hpp>
#include <game/Util.hpp>

#include <vulkan/vulkan.hpp>

namespace snake::core::api {
    std::vector<vk::Framebuffer> make_default_framebuffers(const VulkanContext& ctx) {
        std::vector<vk::Framebuffer> framebuffers{};
        framebuffers.reserve(ctx.swapchain.image_count);

        vk::FramebufferCreateInfo framebuffer_create_info{}; {
            framebuffer_create_info.renderPass = ctx.default_render_pass;
            framebuffer_create_info.height = ctx.swapchain.extent.height;
            framebuffer_create_info.width = ctx.swapchain.extent.width;
            framebuffer_create_info.layers = 1;
        }

        for (const auto& image_view : ctx.swapchain.image_views) {
            framebuffer_create_info.attachmentCount = 1;
            framebuffer_create_info.pAttachments = &image_view;

            framebuffers.emplace_back(ctx.device.logical.createFramebuffer(framebuffer_create_info, nullptr, ctx.dispatcher));
        }

        logger::info("Framebuffers successfully created with default renderpass");

        return framebuffers;
    }
} // namespace snake::core::api