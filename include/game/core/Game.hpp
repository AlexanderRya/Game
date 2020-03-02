#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/Window.hpp>

namespace game::core {
    class Game {
        Window window;
        api::VulkanContext vulkan_context;
        api::Renderer renderer;
    public:
        Game();
    };
} // namespace game::core

#endif //GAME_GAME_HPP
