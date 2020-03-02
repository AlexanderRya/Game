#ifndef GAME_RENDERGRAPH_HPP
#define GAME_RENDERGRAPH_HPP

#include <vulkan/vulkan.hpp>

namespace game::core::api {
    struct RenderGraph {
        vk::ClearColorValue clear_color{};
    };
} // namespace game::core::api

#endif //GAME_RENDERGRAPH_HPP
