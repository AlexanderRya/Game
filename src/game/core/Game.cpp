#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/GameObject.hpp>
#include <game/core/components/Transform.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/gameplay/Level.hpp>
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

        auto level1 = gameplay::load("../resources/levels/level1.lvl", Window::width, Window::height / 2.f);

        graph.entities.emplace_back(registry.create()); {
            registry.assign<components::CameraData>(graph.entities.back());
        }

        graph.entities.emplace_back(registry.create()); {
            components::GameObject background{}; {
                background.vertex_buffer_idx = 1;
                background.vertex_count = 6;
            }

            registry.assign<components::GameObject>(graph.entities.back(), background);
            registry.assign<components::Transform>(graph.entities.back(), components::Transform{
                .instances = {
                    components::Instance{
                        .position = { 0.0f, -Window::height, 0.0f },
                        .size = { Window::width, Window::height, 0.0f },
                        .color = { 1.0f, 1.0f, 1.0f },
                        .rotation = 0
                    }
                }
            });
            registry.assign<components::Texture>(graph.entities.back(), graph.textures[0]);
        }

        graph.entities.emplace_back(registry.create()); { // Bricks
            components::GameObject bricks{}; {
                bricks.vertex_buffer_idx = 1;
                bricks.vertex_count = 6;
            }

            registry.assign<components::GameObject>(graph.entities.back(), bricks);
            registry.assign<components::Transform>(graph.entities.back(), std::move(level1.transforms));
            registry.assign<components::Texture>(graph.entities.back(), graph.textures[1]);
            registry.assign<components::Brick>(graph.entities.back());
        }

        graph.entities.emplace_back(registry.create()); { // Solid Bricks
            components::GameObject solid_bricks{}; {
                solid_bricks.vertex_buffer_idx = 1;
                solid_bricks.vertex_count = 6;
            }

            registry.assign<components::GameObject>(graph.entities.back(), solid_bricks);
            registry.assign<components::Transform>(graph.entities.back(), std::move(level1.solid_bricks_transforms));
            registry.assign<components::Texture>(graph.entities.back(), graph.textures[2]);
            registry.assign<components::Brick>(graph.entities.back());
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