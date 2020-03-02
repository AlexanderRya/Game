#ifndef GAME_DESCRIPTORPOOL_HPP
#define GAME_DESCRIPTORPOOL_HPP

#include <game/Forwards.hpp>

namespace game::core::api {
    [[nodiscard]] vk::DescriptorPool make_descriptor_pool(const VulkanContext&);
} // namespace game::core::api

#endif //GAME_DESCRIPTORPOOL_HPP
