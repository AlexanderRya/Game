#ifndef GAME_INSTANCE_HPP
#define GAME_INSTANCE_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] vk::Instance make_instance(const VulkanContext&);
    [[nodiscard]] vk::DebugUtilsMessengerEXT install_validation_layers(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_INSTANCE_HPP
