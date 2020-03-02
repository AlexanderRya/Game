#include <game/core/Game.hpp>
#include <game/core/api/renderer/RenderGraph.hpp>

namespace game::core {
    Game::Game()
    : window(1280, 720, "Game"),
      vulkan_context(api::make_vulkan_context(&window)),
      renderer(vulkan_context) {

        renderer.init_rendering_data();
    }

    void Game::run() {
        while (!window.should_close()) {
            api::RenderGraph graph{}; {
                graph.clear_color = std::array{ 1.0f, 0.0f, 0.0f, 0.0f };
            }

            renderer.acquire_frame();
            renderer.build(graph);
            renderer.draw();
            window.poll_events();
        }
    }
}