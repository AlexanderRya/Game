#include <game/core/api/imgui/ImGuiContext.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/Window.hpp>
#include <game/Logger.hpp>

#include <imgui/imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace game::core::api::imgui {
    [[nodiscard]] static inline vk::RenderPass make_imgui_render_pass(const VulkanContext& ctx) {
        vk::AttachmentDescription color_description{}; {
            color_description.format = ctx.swapchain.format.format;
            color_description.samples = vk::SampleCountFlagBits::e1;
            color_description.loadOp = vk::AttachmentLoadOp::eLoad;
            color_description.storeOp = vk::AttachmentStoreOp::eStore;
            color_description.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
            color_description.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
            color_description.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
            color_description.finalLayout = vk::ImageLayout::ePresentSrcKHR;
        }

        vk::AttachmentReference color_attachment{}; {
            color_attachment.layout = vk::ImageLayout::eColorAttachmentOptimal;
            color_attachment.attachment = 0;
        }

        vk::SubpassDescription subpass_description{}; {
            subpass_description.colorAttachmentCount = 1;
            subpass_description.pColorAttachments = &color_attachment;
            subpass_description.pDepthStencilAttachment = nullptr;
            subpass_description.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        }

        vk::SubpassDependency subpass_dependency{}; {
            subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpass_dependency.dstSubpass = 0;
            subpass_dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpass_dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpass_dependency.srcAccessMask = {};
            subpass_dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        }

        std::array<vk::AttachmentDescription, 1> attachments {
            color_description
        };

        vk::RenderPassCreateInfo render_pass_create_info{}; {
            render_pass_create_info.attachmentCount = attachments.size();
            render_pass_create_info.pAttachments = attachments.data();
            render_pass_create_info.subpassCount = 1;
            render_pass_create_info.pSubpasses = &subpass_description;
            render_pass_create_info.dependencyCount = 1;
            render_pass_create_info.pDependencies = &subpass_dependency;
        }

        auto render_pass = ctx.device.logical.createRenderPass(render_pass_create_info, nullptr, ctx.dispatcher);

        logger::info("ImGui renderpass successfully created");

        return render_pass;
    }

    [[nodiscard]] static inline std::vector<vk::CommandBuffer> make_imgui_command_buffers(const VulkanContext& ctx) {
        vk::CommandBufferAllocateInfo allocate_info{}; {
            allocate_info.commandPool = ctx.command_pool;
            allocate_info.commandBufferCount = ctx.swapchain.image_count;
            allocate_info.level = vk::CommandBufferLevel::ePrimary;
        }

        auto buffers = ctx.device.logical.allocateCommandBuffers(allocate_info, ctx.dispatcher);

        logger::info("Created ", allocate_info.commandBufferCount, " command buffers for imgui");

        return buffers;
    }

    ImGuiContext make_imgui_context(const Window& window, const VulkanContext& ctx) {
        ImGuiContext imgui_ctx{};

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        imgui_ctx.render_pass = make_imgui_render_pass(ctx);
        imgui_ctx.command_buffers = make_imgui_command_buffers(ctx);

        ImGui_ImplGlfw_InitForVulkan(window.handle(), false);

        ImGui_ImplVulkan_InitInfo imgui_vulkan_init_info{}; {
            imgui_vulkan_init_info.QueueFamily = ctx.device.queue_family;
            imgui_vulkan_init_info.Queue = ctx.device.queue;
            imgui_vulkan_init_info.PhysicalDevice = ctx.device.physical;
            imgui_vulkan_init_info.MinImageCount = ctx.swapchain.image_count - 1;
            imgui_vulkan_init_info.ImageCount = ctx.swapchain.image_count;
            imgui_vulkan_init_info.Device = ctx.device.logical;
            imgui_vulkan_init_info.MSAASamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
            imgui_vulkan_init_info.Allocator = nullptr;
            imgui_vulkan_init_info.Instance = ctx.instance;
            imgui_vulkan_init_info.PipelineCache = nullptr;
            imgui_vulkan_init_info.DescriptorPool = ctx.descriptor_pool;
            imgui_vulkan_init_info.CheckVkResultFn = nullptr;
        }

        ImGui_ImplVulkan_Init(&imgui_vulkan_init_info, static_cast<VkRenderPass>(imgui_ctx.render_pass));

        auto command_buffer = begin_transient(ctx); {
            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
        } end_transient(ctx, command_buffer);

        return imgui_ctx;
    }
} // namespace game::core::api::imgui