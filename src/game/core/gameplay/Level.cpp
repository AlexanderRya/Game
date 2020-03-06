#include <game/core/components/GameObject.hpp>
#include <game/core/gameplay/Level.hpp>
#include <game/Types.hpp>

#include <stdexcept>
#include <fstream>
#include <vector>

namespace game::core::gameplay {
    [[nodiscard]] static inline std::vector<std::string> split(const std::string& str, const std::string& delim) {
        std::vector<std::string> splitted;

        size_t pos_start = 0, pos_end, delim_len = delim.length();

        while ((pos_end = str.find(delim, pos_start)) != std::string::npos) {
            auto token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            splitted.emplace_back(token);
        }

        splitted.emplace_back(str.substr(pos_start));

        return splitted;
    }

    std::vector<components::GameObject> load_level(const std::filesystem::path& level, const f64 w_width, const f64 w_height) {
        std::vector<components::GameObject> objects{};

        std::ifstream in(level);

        if (!in.is_open()) {
            throw std::runtime_error("Failed to open file at: " + level.generic_string());
        }

        std::vector<std::vector<std::string>> tiles{};

        std::string line{};
        while (std::getline(in, line, '\n')) {
            tiles.emplace_back(split(line, " "));
        }

        objects.emplace_back(components::GameObject{ // Background
            .vertex_count = 6,
            .vertex_buffer_id = 1,
            .texture_idx = 2,
            .info = {
                GameObjectInfo{
                    .position = { 0.0f, -w_height * 2 },
                    .size = { w_width, w_height * 2 },
                    .color = { 1.0f, 1.0f, 1.0f }
                }
            }
        });

        objects.emplace_back(components::GameObject{ // Paddle
            .vertex_count = 6,
            .vertex_buffer_id = 1,
            .texture_idx = 3,
            .info = {
                GameObjectInfo{
                    .position = {
                        w_width / 2.f - 100.f / 2.f,
                        -20
                    },
                    .size = { 100, 20 },
                    .color = { 1.0f, 1.0f, 1.0f }
                }
            }
        });


        objects.emplace_back(components::GameObject{
            .vertex_count = 6,
            .vertex_buffer_id = 1,
            .texture_idx = 0
        });

        objects.emplace_back(components::GameObject{
            .vertex_count = 6,
            .vertex_buffer_id = 1,
            .texture_idx = 1
        });

        u64 height = tiles.size();
        u64 width = tiles[0].size();
        f32 unit_width = w_width / static_cast<f32>(width);
        f32 unit_height = w_height / static_cast<f32>(height);
        for (u64 y = 0; y < height; ++y) {
            for (u64 x = 0; x < width; ++x) {
                if (tiles[y][x] == "1")  { // Solid
                    glm::vec2 pos(unit_width * x, (unit_height * y) - w_height * 2);
                    glm::vec2 size(unit_width, unit_height);

                    objects[3].info.emplace_back(GameObjectInfo{
                        .is_solid = true,
                        .size = size,
                        .position = pos,
                        .color = { 0.8f, 0.8f, 0.7f }
                    });
                } else if (tiles[y][x] > "1") {
                    auto color = glm::vec3(1.0f); // Original: white

                    if (tiles[y][x] == "2") {
                        color = glm::vec3(0.2f, 0.6f, 1.0f);
                    } else if (tiles[y][x] == "3") {
                        color = glm::vec3(0.0f, 0.7f, 0.0f);
                    } else if (tiles[y][x] == "4") {
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                    } else if (tiles[y][x] == "5") {
                        color = glm::vec3(1.0f, 0.5f, 0.0f);
                    }

                    glm::vec2 pos(unit_width * x, (unit_height * y) - w_height * 2);
                    glm::vec2 size(unit_width, unit_height);

                    objects[2].info.emplace_back(GameObjectInfo{
                        .is_solid = false,
                        .size = size,
                        .position = pos,
                        .color = color
                    });
                }
            }
        }
        return objects;
    }
} // namespace game::core::gameplay