#ifndef SNAKE_SWAPCHAIN_HPP
#define SNAKE_SWAPCHAIN_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] Swapchain make_swapchain(const Window*, const VulkanContext&);
} // namespace snake::core::apis

#endif //SNAKE_SWAPCHAIN_HPP
