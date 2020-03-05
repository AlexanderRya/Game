#ifndef GAME_SAMPLER_HPP
#define GAME_SAMPLER_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

namespace game::core::api {
    [[nodiscard]] vk::Sampler make_default_sampler(const VulkanContext&);
}

#endif //GAME_SAMPLER_HPP
