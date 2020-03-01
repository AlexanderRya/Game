#ifndef SNAKE_COMMANDPOOL_HPP
#define SNAKE_COMMANDPOOL_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] vk::CommandPool make_command_pool(const VulkanContext&);
    [[nodiscard]] vk::CommandPool make_transient_pool(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_COMMANDPOOL_HPP
