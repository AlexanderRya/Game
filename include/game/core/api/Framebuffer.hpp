#ifndef SNAKE_FRAMEBUFFER_HPP
#define SNAKE_FRAMEBUFFER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace snake::core::api {
    [[nodiscard]] std::vector<vk::Framebuffer> make_default_framebuffers(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_FRAMEBUFFER_HPP
