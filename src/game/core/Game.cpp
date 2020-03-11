#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/GameObject.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/api/Sampler.hpp>
#include <game/core/Globals.hpp>
#include <game/core/Game.hpp>

#include <entt/entt.hpp>

#include <iostream>
#include <fstream>

namespace game::core {
    Game::Game()
    : window(1280, 720, "Game"),
      context(api::make_vulkan_context(&window)),
      renderer(context) {
        renderer.init_rendering_data();
    }

    void Game::run() {
        api::renderer::RenderGraph graph{};
        entt::registry registry{};

        graph.textures.reserve(4);
        graph.textures.emplace_back(&context).load("../resources/textures/background.jpg");
        graph.textures.emplace_back(&context).load("../resources/textures/block.png");
        graph.textures.emplace_back(&context).load("../resources/textures/block_solid.png");
        graph.textures.emplace_back(&context).load("../resources/textures/paddle.png");

        graph.main_camera = registry.create(); {
            registry.assign<components::CameraData>(graph.main_camera);
        }

        graph.objects.emplace_back(registry.create()); {
            components::GameObject bricks{}; {
                bricks.vertex_buffer_idx = 1;
                bricks.vertex_count = 6;

                bricks.transforms = {
                    components::Transform {
                        .position = { 0.0f, -Window::height * 2, 0.0f },
                        .size = { Window::width, Window::height * 2, 0.0f },
                        .color = { 1.0f, 1.0f, 1.0f }
                    }
                };
            }

            registry.assign<components::GameObject>(graph.objects.back(), bricks);
            registry.assign<components::Texture>(graph.objects.back(), graph.textures[0]);
        }

        renderer.build(graph, registry);

        while (!window.should_close()) {
            f32 frame_time = glfwGetTime();
            delta_time = frame_time - last_frame;
            last_frame = frame_time;

            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            window.poll_events();

            renderer.acquire_frame();

            renderer.start(); {
                renderer.draw(graph, registry);
            } renderer.end();

            renderer.submit();
        }
    }

    Game::~Game() {
        char* str = new char[1000000]{};
        vmaBuildStatsString(context.allocator, &str, true);

        std::ofstream("../dump/stats.json") << str;

        vmaFreeStatsString(context.allocator, str);
    }
}