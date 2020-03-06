#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/gameplay/Level.hpp>
#include <game/core/api/Sampler.hpp>
#include <game/core/Globals.hpp>
#include <game/core/Game.hpp>

#include <iostream>
#include <fstream>

namespace game::core {
    Game::Game()
    : window(800, 600, "Game"),
      context(api::make_vulkan_context(&window)),
      renderer(context) {
        renderer.init_rendering_data();
    }

    void Game::run() {
        api::RenderGraph graph{}; {
            graph.clear_values[0].color = std::array{ 0.02f, 0.02f, 0.02f, 0.0f };
            graph.clear_values[1].depthStencil = { { 1.0f, 0 } };

            graph.layouts[meta::PipelineLayoutType::MeshGeneric] = api::make_generic_pipeline_layout(context);

            graph.samplers[meta::SamplerType::Default] = api::make_default_sampler(context);

            graph.textures.emplace_back(context, graph.samplers[meta::SamplerType::Default]).load("../resources/textures/block.png");
            graph.textures.emplace_back(context, graph.samplers[meta::SamplerType::Default]).load("../resources/textures/block_solid.png");
            graph.textures.emplace_back(context, graph.samplers[meta::SamplerType::Default]).load("../resources/textures/background.jpg");
            graph.textures.emplace_back(context, graph.samplers[meta::SamplerType::Default]).load("../resources/textures/paddle.png");

            api::Pipeline::CreateInfo create_info{}; {
                create_info.ctx = &context;
                create_info.vertex_path = "../resources/shaders/generic.vert.spv";
                create_info.fragment_path = "../resources/shaders/generic.frag.spv";
                create_info.layout = graph.layouts[meta::PipelineLayoutType::MeshGeneric];
            }

            graph.pipelines[meta::PipelineType::MeshGeneric] = api::make_generic_pipeline(create_info);

            graph.game_objects = gameplay::load_level("../resources/levels/level1.lvl", Window::width, Window::height / 2);
        }

        graph.build(context);

        window.set_user_pointer(&graph);

        while (!window.should_close()) {
            f32 frame_time = glfwGetTime();
            delta_time = frame_time - last_frame;
            last_frame = frame_time;

            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            window.poll_events();

            renderer.acquire_frame();
            renderer.build(graph);
            renderer.draw();
        }
    }

    Game::~Game() {
        char* str = new char[1000000]{};
        vmaBuildStatsString(context.allocator, &str, true);

        std::ofstream("../dump/stats.json") << str;

        vmaFreeStatsString(context.allocator, str);
    }
}