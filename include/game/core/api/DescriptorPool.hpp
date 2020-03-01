#ifndef SNAKE_DESCRIPTORPOOL_HPP
#define SNAKE_DESCRIPTORPOOL_HPP

#include <game/Forwards.hpp>

namespace snake::core::api {
    [[nodiscard]] vk::DescriptorPool make_descriptor_pool(const VulkanContext&);
} // namespace snake::core::api

#endif //SNAKE_DESCRIPTORPOOL_HPP
