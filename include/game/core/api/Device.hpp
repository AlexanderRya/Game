#ifndef GAME_DEVICE_HPP
#define GAME_DEVICE_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] Device make_device(const VulkanContext&);
    [[nodiscard]] u64 find_memory_type(const VulkanContext&, const u32, const vk::MemoryPropertyFlags&);
} // namespace game::core::api

#endif //GAME_DEVICE_HPP
