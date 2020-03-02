#include <game/core/Game.hpp>

namespace game::core {
    Game::Game()
    : window(1280, 720, "Game"),
      vulkan_context(api::make_vulkan_context(&window)),
      renderer(vulkan_context) {

    }
}