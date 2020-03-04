#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/Globals.hpp>
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

            graph.layouts[meta::PipelineLayoutType::MeshGeneric] = api::make_generic_pipeline_layout(context);

            api::Pipeline::CreateInfo create_info{}; {
                create_info.ctx = &context;
                create_info.vertex_path = "../resources/shaders/generic.vert.spv";
                create_info.fragment_path = "../resources/shaders/generic.frag.spv";
                create_info.layout = graph.layouts[meta::PipelineLayoutType::MeshGeneric];
            }

            graph.pipelines[0] = api::make_generic_pipeline(create_info);

            graph.meshes.emplace_back(components::Mesh{
                .vertex_count = 3,
                .vertex_buffer_id = 0
            });
        }

        graph.build(context);

        while (!window.should_close()) {
            f32 current_frame = glfwGetTime();
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            camera.move(window);

            renderer.acquire_frame();
            renderer.build(graph);
            renderer.draw();

            window.poll_events();
        }
    }
}