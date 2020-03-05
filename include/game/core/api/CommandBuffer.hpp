#ifndef GAME_COMMANDBUFFER_HPP
#define GAME_COMMANDBUFFER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace game::core::api {
    [[nodiscard]] std::vector<vk::CommandBuffer> make_rendering_command_buffers(const VulkanContext&);
    [[nodiscard]] vk::CommandBuffer begin_transient(const VulkanContext&);
    void end_transient(const VulkanContext&, const vk::CommandBuffer);
} // namespace game::core::api

#endif //GAME_COMMANDBUFFER_HPP
