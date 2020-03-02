#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/api/VulkanContext.hpp>

namespace game::core::api {
    Renderer::Renderer(const game::core::api::VulkanContext& context)
    : ctx(context) {
        vk::SemaphoreCreateInfo semaphore_create_info{};

        image_available.resize(ctx.swapchain.image_count, ctx.device.logical.createSemaphore(semaphore_create_info, nullptr, ctx.dispatcher));
        render_finished.resize(ctx.swapchain.image_count, ctx.device.logical.createSemaphore(semaphore_create_info, nullptr, ctx.dispatcher));
    }
} // namespace game::core::api
