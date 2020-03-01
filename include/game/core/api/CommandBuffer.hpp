#ifndef SNAKE_COMMANDBUFFER_HPP
#define SNAKE_COMMANDBUFFER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace snake::core::api {
    [[nodiscard]] std::vector<vk::CommandBuffer> make_rendering_command_buffers(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_COMMANDBUFFER_HPP
