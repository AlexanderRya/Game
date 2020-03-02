#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/Game.hpp>

namespace game::core {
    Game::Game()
    : window(1280, 720, "Game"),
      context(api::make_vulkan_context(&window)),
      renderer(context) {

        renderer.init_rendering_data();
    }

    void Game::run() {
        api::RenderGraph graph{}; {
            graph.clear_color = std::array{ 0.02f, 0.02f, 0.02f, 0.0f };

            graph.pipeline_layouts[0] = api::make_generic_pipeline_layout(context);

            api::Pipeline::CreateInfo create_info{}; {
                create_info.ctx = &context;
                create_info.vertex_path = "../resources/shaders/generic.vert.spv";
                create_info.fragment_path = "../resources/shaders/generic.frag.spv";
                create_info.layout = graph.pipeline_layouts[0];
            }

            graph.pipelines[0] = api::make_generic_pipeline(create_info);

            graph.meshes.emplace_back(components::Mesh{
                .vertex_count = 3,
                .vertex_buffer_id = 0
            });
        }

        while (!window.should_close()) {
            renderer.acquire_frame();
            renderer.build(graph);
            renderer.draw();

            window.poll_events();
        }
    }
}