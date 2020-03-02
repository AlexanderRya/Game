#ifndef GAME_COMMANDPOOL_HPP
#define GAME_COMMANDPOOL_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] vk::CommandPool make_command_pool(const VulkanContext&);
    [[nodiscard]] vk::CommandPool make_transient_pool(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_COMMANDPOOL_HPP
