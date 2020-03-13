#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/GameObject.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/gameplay/GameLevel.hpp>
#include <game/core/components/Camera.hpp>
#include <game/core/components/Player.hpp>
#include <game/core/components/Ball.hpp>
#include <game/core/Window.hpp>

#include <type_traits>
#include <fstream>
#include <string>

namespace game::core::gameplay {
    template <typename Ty>
    [[nodiscard]] static inline std::vector<Ty> split(const std::string& str, const std::string& delim) {
        std::vector<Ty> splitted;

        size_t pos_start = 0, pos_end, delim_len = delim.length();

        while ((pos_end = str.find(delim, pos_start)) != std::string::npos) {
            auto token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            if constexpr (std::is_same_v<Ty, i32>) {
                splitted.emplace_back(std::stoi(token));
            } else {
                splitted.emplace_back(token);
            }
        }
        if constexpr (std::is_same_v<Ty, i32>) {
            splitted.emplace_back(std::stoi(str.substr(pos_start)));
        } else {
            splitted.emplace_back(str.substr(pos_start));
        }

        return splitted;
    }

    void GameLevel::load(const std::filesystem::path& path, const f64 w_width, const f64 w_height) {
        graph.textures.reserve(5);
        graph.textures.emplace_back(&ctx).load("../resources/textures/background.jpg");
        graph.textures.emplace_back(&ctx).load("../resources/textures/block.png");
        graph.textures.emplace_back(&ctx).load("../resources/textures/block_solid.png");
        graph.textures.emplace_back(&ctx).load("../resources/textures/paddle.png");
        graph.textures.emplace_back(&ctx).load("../resources/textures/awesome_face.png");

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
                    components::Transform::Instance{
                        .position = { 0.0f, -Window::height, 0.0f },
                        .size = { Window::width, Window::height, 0.0f },
                        .color = { 1.0f, 1.0f, 1.0f },
                        .rotation = 0
                    }
                }
            });
            registry.assign<components::Texture>(graph.entities.back(), graph.textures[0]);
        }

        std::ifstream in(path);

        if (!in.is_open()) {
            throw std::runtime_error("Failed to open file at: " + path.generic_string());
        }

        std::vector<std::vector<int>> tiles{};

        std::string line{};
        while (std::getline(in, line, '\n')) {
            tiles.emplace_back(split<i32>(line, " "));
        }

        u64 height = tiles.size();
        u64 width = tiles[0].size();

        f32 unit_width = w_width / static_cast<f32>(width);
        f32 unit_height = w_height / static_cast<f32>(height);
        /* Bricks loading */ {
            auto& solid_bricks = graph.entities.emplace_back(registry.create());
            auto& bricks = graph.entities.emplace_back(registry.create());

            registry.assign<components::Texture>(bricks, graph.textures[1]);
            registry.assign<components::Texture>(solid_bricks, graph.textures[2]);

            registry.assign<components::GameObject>(bricks, components::GameObject{
                .vertex_buffer_idx = 1,
                .vertex_count = 6
            });
            registry.assign<components::Brick>(bricks);
            registry.assign<components::Transform>(bricks);

            registry.assign<components::GameObject>(solid_bricks, components::GameObject{
                .vertex_buffer_idx = 1,
                .vertex_count = 6
            });
            registry.assign<components::Brick>(solid_bricks);
            registry.assign<components::Transform>(solid_bricks);

            for (u64 y = 0; y < height; ++y) {
                for (u64 x = 0; x < width; ++x) {
                    if (tiles[y][x] == 1) { // Solid
                        glm::vec3 pos(unit_width * x, (unit_height * y) - w_height * 2, 0.0f);
                        glm::vec3 size(unit_width, unit_height, 0.0f);

                        registry.get<components::Brick>(solid_bricks).info.emplace_back(components::Brick::Info{
                            .is_solid = true
                        });

                        registry.get<components::Transform>(solid_bricks).instances.emplace_back(components::Transform::Instance{
                            .size = size,
                            .position = pos,
                            .color = { 0.8f, 0.8f, 0.7f }
                        });
                    } else if (tiles[y][x] > 1) {
                        auto color = glm::vec3(1.0f); // Original: white

                        if (tiles[y][x] == 2) {
                            color = glm::vec3(0.2f, 0.6f, 1.0f);
                        } else if (tiles[y][x] == 3) {
                            color = glm::vec3(0.0f, 0.7f, 0.0f);
                        } else if (tiles[y][x] == 4) {
                            color = glm::vec3(0.8f, 0.8f, 0.4f);
                        } else if (tiles[y][x] == 5) {
                            color = glm::vec3(1.0f, 0.5f, 0.0f);
                        }

                        glm::vec3 pos(unit_width * x, (unit_height * y) - w_height * 2, 0.0f);
                        glm::vec3 size(unit_width, unit_height, 0.0f);

                        registry.get<components::Brick>(bricks).info.emplace_back(components::Brick::Info{
                            .is_solid = false
                        });

                        registry.get<components::Transform>(bricks).instances.emplace_back(components::Transform::Instance{
                            .size = size,
                            .position = pos,
                            .color = color
                        });
                    }
                }
            }
        }

        constexpr glm::vec3 player_size = { 150.f, 25, 0 };
        glm::vec3 player_position = {
            Window::width / 2.f - player_size.x / 2.f,
            -player_size.y,
            0.0f
        };
        constexpr float ball_radius = 17.5f;

        /* Player */ {
            auto& player = graph.entities.emplace_back(registry.create());

            registry.assign<components::GameObject>(player, components::GameObject{
                .vertex_buffer_idx = 1,
                .vertex_count = 6
            });
            registry.assign<components::Transform>(player, components::Transform{
                .instances = {
                    components::Transform::Instance{
                        .size = { player_size },
                        .position = player_position,
                        .color = { 1.0f, 1.0f, 1.0f }
                    }
                }
            });
            registry.assign<components::Texture>(player, graph.textures[3]);
            registry.assign<components::Player>(player);
        }

        /* Ball */ {
            auto& ball = graph.entities.emplace_back(registry.create());

            registry.assign<components::GameObject>(ball, components::GameObject{
                .vertex_buffer_idx = 1,
                .vertex_count = 6
            });
            registry.assign<components::Transform>(ball, components::Transform{
                .instances = {
                    components::Transform::Instance{
                        .size = { ball_radius * 2, ball_radius * 2, 0 },
                        .position = player_position + glm::vec3(player_size.x / 2.f - ball_radius, -ball_radius * 2.f, 0.0f),
                        .color = { 1.0f, 1.0f, 1.0f }
                    }
                }
            });
            registry.assign<components::Texture>(ball, graph.textures[4]);
            registry.assign<components::Ball>(ball, components::Ball{
                .velocity = { 100.0f, -350.0f, 0.0f },
                .is_stuck = true,
                .radius = ball_radius
            });
        }
    }
} // namespace game::core::gameplay