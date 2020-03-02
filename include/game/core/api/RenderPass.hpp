#ifndef GAME_RENDERPASS_HPP
#define GAME_RENDERPASS_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] vk::RenderPass make_default_render_pass(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_RENDERPASS_HPP
