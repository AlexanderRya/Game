#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/api/VulkanContext.hpp>

namespace snake::core::api {
    Renderer::Renderer(const snake::core::api::VulkanContext& ctx)
        : ctx(ctx) {

        vk::SemaphoreCreateInfo semaphore_create_info{};

        image_available.resize(ctx.swapchain.image_count, ctx.device.logical.createSemaphore(semaphore_create_info));
        render_finished.resize(ctx.swapchain.image_count, ctx.device.logical.createSemaphore(semaphore_create_info));
    }
} // namespace snake::core::api
