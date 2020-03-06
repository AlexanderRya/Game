#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/core/Window.hpp>
#include <game/Forwards.hpp>

#include <unordered_map>

namespace game::core {
    class Game {
        Window window;
        api::VulkanContext context;
        api::Renderer renderer;
    public:
        Game();
        ~Game();

        void run();
    };
} // namespace game::core

#endif //GAME_GAME_HPP
