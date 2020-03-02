#ifndef GAME_FRAMEBUFFER_HPP
#define GAME_FRAMEBUFFER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace game::core::api {
    [[nodiscard]] std::vector<vk::Framebuffer> make_default_framebuffers(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_FRAMEBUFFER_HPP
