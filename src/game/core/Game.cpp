#include <game/core/components/GameObject.hpp>
#include <game/core/components/Transform.hpp>
#include <game/core/gameplay/GameLevel.hpp>
#include <game/core/components/Player.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/components/Ball.hpp>
#include <game/core/api/Sampler.hpp>
#include <game/core/Globals.hpp>
#include <game/core/Game.hpp>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <fstream>

namespace game::core {
    Game::Game()
    : window(1600, 900, "Game"),
      context(api::make_vulkan_context(&window)),
      imgui_context(api::imgui::make_imgui_context(window, context)),
      renderer(context, imgui_context) {
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
        const auto ball = registry.view<components::Transform, components::Ball>();

        const auto& ball_instance = ball.get<components::Transform>(*ball.begin()).instances[0];
        auto& ball_info = ball.get<components::Ball>(*ball.begin());

        for (const auto& each : bricks) {
            auto& brick_instances = bricks.get<components::Transform>(each).instances;
            auto& brick_info = bricks.get<components::Brick>(each).info;

            for (usize i = 0; i < brick_instances.size(); ++i) {
                glm::vec2 center(ball_instance.position + ball_info.radius);

                glm::vec2 aabb_half_extents(brick_instances[i].size.x / 2, brick_instances[i].size.y / 2);
                glm::vec2 aabb_center(
                    brick_instances[i].position.x + aabb_half_extents.x,
                    brick_instances[i].position.y + aabb_half_extents.y);

                glm::vec2 difference = center - aabb_center;
                glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

                glm::vec2 closest = aabb_center + clamped;

                difference = closest - center;

                if (glm::length(difference) < ball_info.radius && !brick_info[i].is_solid) {
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

        auto ball_view = levels[level_idx].registry.view<components::Ball, components::Transform>();

        while (!window.should_close()) {
            f32 frame_time = glfwGetTime();
            delta_time = frame_time - last_frame;
            last_frame = frame_time;

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowSize({ 400.0f, 0.0f });
            ImGui::SetNextWindowPos({ static_cast<float>(context.swapchain.extent.width - 400), 0.0f });
            ImGui::Begin("Debug info");
            ImGui::Text("fps: %.2f, delta_time: %.5f, frame: %llu",
                        1 / delta_time,
                        delta_time,
                        renderer.frames_rendered);
            ImGui::End();
            auto& ball_instance = ball_view.get<components::Transform>(*ball_view.begin()).instances[0];
            ImGui::SetNextWindowSize({ 250.0f, 0.0f });
            ImGui::SetNextWindowPos({ static_cast<float>(context.swapchain.extent.width - 250), 47.0f });
            ImGui::Begin("Ball info");
            ImGui::Text("x: %.2f, y: %.2f, z: %.2f",
                        ball_instance.position.x,
                        ball_instance.position.y,
                        ball_instance.position.z);
            ImGui::End();
            ImGui::Render();

            window.poll_events();

            renderer.acquire_frame();

            process_collisions(levels[level_idx]);
            process_input(levels[level_idx]);

            renderer.start(); {
                renderer.draw(levels[level_idx]);
            } renderer.end();

            renderer.draw_imgui();

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