#ifndef GAME_SWAPCHAIN_HPP
#define GAME_SWAPCHAIN_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] Swapchain make_swapchain(const Window*, const VulkanContext&);
} // namespace game::core::api

#endif //GAME_SWAPCHAIN_HPP
