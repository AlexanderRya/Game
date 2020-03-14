#ifndef GAME_IMGUICONTEXT_HPP
#define GAME_IMGUICONTEXT_HPP

#include <game/Forwards.hpp>

#include <vulkan/vulkan.hpp>

namespace game::core::api::imgui {
    struct ImGuiContext {
        vk::RenderPass render_pass;
        std::vector<vk::CommandBuffer> command_buffers;
    };

    [[nodiscard]] ImGuiContext make_imgui_context(const Window&, const VulkanContext&);
} // namespace game::core::api::imgui

#endif //GAME_IMGUICONTEXT_HPP
