#ifndef GAME_COMMANDBUFFER_HPP
#define GAME_COMMANDBUFFER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace game::core::api {
    [[nodiscard]] std::vector<vk::CommandBuffer> make_rendering_command_buffers(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_COMMANDBUFFER_HPP
