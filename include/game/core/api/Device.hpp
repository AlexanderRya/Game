#ifndef GAME_DEVICE_HPP
#define GAME_DEVICE_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] Device make_device(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_DEVICE_HPP
