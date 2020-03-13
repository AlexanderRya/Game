#include <game/core/components/GameObject.hpp>
#include <game/core/components/Transform.hpp>
#include <game/core/gameplay/GameLevel.hpp>
#include <game/core/components/Player.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/components/Ball.hpp>
#include <game/core/api/Sampler.hpp>
#include <game/core/Globals.hpp>
#include <game/core/Game.hpp>
#include <game/Util.hpp>

#include <iostream>
#include <fstream>

namespace game::core {
    Game::Game()
    : window(1600, 900, "Game"),
      context(api::make_vulkan_context(&window)),
      renderer(context) {
        renderer.init_rendering_data();
    }

    void Game::load() {
        levels.emplace_back(context).load("../resources/levels/level1.lvl", Window::width, Window::height / 2.f);

        for (auto& level : levels) {
            renderer.build(level.graph, level.registry);
        }
    }

    void Game::process_collisions(gameplay::GameLevel& level) {
        auto& registry = level.registry;

        const auto bricks = registry.view<components::Transform, components::Brick>();
        auto ball = registry.view<components::Transform, components::Ball>();

        const auto& ball_instance = ball.get<components::Transform>(*ball.begin()).instances[0];

        for (const auto& each : bricks) {
            auto& brick_instances = bricks.get<components::Transform>(each).instances;
            auto& brick_info = bricks.get<components::Brick>(each).info;

            for (usize i = 0; i < brick_instances.size(); ++i) {
                bool x_collision =
                    ball_instance.position.x + ball_instance.size.x >= brick_instances[i].position.x &&
                    brick_instances[i].position.x + brick_instances[i].size.x >= ball_instance.position.x;

                bool y_collision =
                    ball_instance.position.y + ball_instance.size.y >= brick_instances[i].position.y &&
                    brick_instances[i].position.y + brick_instances[i].size.y >= ball_instance.position.y;

                if (x_collision && y_collision && !brick_info[i].is_solid) {
                    brick_info.erase(brick_info.begin() + i);
                    brick_instances.erase(brick_instances.begin() + i);
                }
            }
        }
    }

    void Game::process_input(gameplay::GameLevel& level) {
        auto& registry = level.registry;

        auto player = registry.view<components::Transform, components::Player>();
        auto ball = registry.view<components::Transform, components::Ball>();

        auto& player_instance = player.get<components::Transform>(*player.begin()).instances[0];
        auto& ball_instance = ball.get<components::Transform>(*ball.begin()).instances[0];

        auto& player_info = player.get<components::Player>(*player.begin());
        auto& ball_info = ball.get<components::Ball>(*ball.begin());

        auto player_velocity = player_info.velocity * delta_time;

        // Player movement
        if (window.get_key(GLFW_KEY_A) == GLFW_PRESS) {
            if (player_instance.position.x >= 0) {
                player_instance.position.x -= player_velocity;
                if (ball_info.is_stuck) {
                    ball_instance.position.x -= player_velocity;
                }
            }
        }

        if (window.get_key(GLFW_KEY_D) == GLFW_PRESS) {
            if (player_instance.position.x <= Window::width - player_instance.size.x) {
                player_instance.position.x += player_velocity;
                if (ball_info.is_stuck) {
                    ball_instance.position.x += player_velocity;
                }
            }
        }

        // Ball movement
        if (!ball_info.is_stuck) {
            ball_instance.position += ball_info.velocity * delta_time;

            if (ball_instance.position.x <= 0.0f) {
                ball_info.velocity.x = -ball_info.velocity.x;
                ball_instance.position.x = 0.0f;
            } else if (ball_instance.position.x + ball_instance.size.x >= Window::width) {
                ball_info.velocity.x = -ball_info.velocity.x;
                ball_instance.position.x = Window::width - ball_instance.size.x;
            }

            if (ball_instance.position.y <= -Window::height) {
                ball_info.velocity.y = -ball_info.velocity.y;
            } else if (ball_instance.position.y + ball_instance.size.y >= 0.0f) {
                ball_info.velocity.y = -ball_info.velocity.y;
                ball_instance.position.y = -ball_instance.size.y;
            }
        }

        if (window.get_key(GLFW_KEY_SPACE) == GLFW_PRESS) {
            ball_info.is_stuck = false;
        }

        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
    }

    void Game::run() {
        constexpr static usize level_idx = 0;

        while (!window.should_close()) {
            f32 frame_time = glfwGetTime();
            delta_time = frame_time - last_frame;
            last_frame = frame_time;

            window.poll_events();

            renderer.acquire_frame();

            process_collisions(levels[level_idx]);
            process_input(levels[level_idx]);

            renderer.start(); {
                renderer.draw(levels[level_idx]);
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