#ifndef SNAKE_RENDERPASS_HPP
#define SNAKE_RENDERPASS_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] vk::RenderPass make_default_render_pass(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_RENDERPASS_HPP
