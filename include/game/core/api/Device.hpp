#ifndef SNAKE_DEVICE_HPP
#define SNAKE_DEVICE_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] Device make_device(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_DEVICE_HPP
