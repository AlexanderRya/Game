#ifndef SNAKE_INSTANCE_HPP
#define SNAKE_INSTANCE_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] vk::Instance make_instance(const VulkanContext&);
    [[nodiscard]] vk::DebugUtilsMessengerEXT install_validation_layers(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_INSTANCE_HPP
