#include <game/core/api/DescriptorPool.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/api/CommandPool.hpp>
#include <game/core/api/Framebuffer.hpp>
#include <game/core/api/RenderPass.hpp>
#include <game/core/api/Swapchain.hpp>
#include <game/core/api/Instance.hpp>
#include <game/core/api/Device.hpp>
#include <game/core/Window.hpp>
#include <game/Logger.hpp>
#include <game/Util.hpp>

namespace snake::core::api {
    static inline void load_vulkan_module(VulkanContext& ctx) {
        auto module = util::load_module(util::vulkan_module);

        logger::info("Vulkan module: ", util::vulkan_module, " loaded at address: ", module);
        ctx.dispatcher.vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(util::load_symbol(module, "vkCreateInstance"));
        ctx.dispatcher.vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(util::load_symbol(module, "vkEnumerateInstanceExtensionProperties"));
        ctx.dispatcher.vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(util::load_symbol(module, "vkGetInstanceProcAddr"));

        util::close_module(module);
    }

    VulkanContext make_vulkan_context(Window* window) {
        VulkanContext ctx{};

        logger::info("Vulkan initialization sequence starting");
        load_vulkan_module(ctx);
        ctx.window = window;
        ctx.instance = make_instance(ctx);
        ctx.dispatcher.init(static_cast<VkInstance>(ctx.instance), ctx.dispatcher.vkGetInstanceProcAddr);
#if SNAKE_DEBUG
        logger::warning("Vulkan debug mode active, performance may be lower than usual");
        ctx.validation = install_validation_layers(ctx);
#endif
        ctx.surface = window->create_surface(ctx);
        ctx.device = make_device(ctx);
        ctx.swapchain = make_swapchain(window, ctx);
        ctx.command_pool = make_command_pool(ctx);
        ctx.transient_pool = make_transient_pool(ctx);
        ctx.command_buffers = make_rendering_command_buffers(ctx);
        ctx.descriptor_pool = make_descriptor_pool(ctx);
        ctx.default_render_pass = make_default_render_pass(ctx);
        ctx.default_framebuffers = make_default_framebuffers(ctx);

        logger::info("Vulkan initialization sequence completed successfully");

        return ctx;
    }
} // namespace snake::core::api