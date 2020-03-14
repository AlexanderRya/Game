#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <game/core/api/imgui/ImGuiContext.hpp>
#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/gameplay/GameLevel.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/core/Window.hpp>
#include <game/Forwards.hpp>

#include <unordered_map>

namespace game::core {
    class Game {
        Window window;
        api::VulkanContext context;
        api::renderer::Renderer renderer;
        api::imgui::ImGuiContext imgui_context;

        std::vector<gameplay::GameLevel> levels;
    public:
        Game();
        ~Game();

        void load();
        void process_collisions(gameplay::GameLevel&);
        void process_input(gameplay::GameLevel&);
        void run();
    };
} // namespace game::core

#endif //GAME_GAME_HPP
