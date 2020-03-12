#include <game/core/gameplay/Level.hpp>

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

    GameLevel load(const std::filesystem::path& path, const f64 w_width, const f64 w_height) {
        GameLevel level{};

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
        for (u64 y = 0; y < height; ++y) {
            for (u64 x = 0; x < width; ++x) {
                if (tiles[y][x] == 1)  { // Solid
                    glm::vec3 pos(unit_width * x, (unit_height * y) - w_height * 2, 0.0f);
                    glm::vec3 size(unit_width, unit_height, 0.0f);

                    level.solid_bricks.info.emplace_back(components::Brick::Info{
                        .is_solid = true
                    });

                    level.solid_bricks_transforms.instances.emplace_back(components::Instance{
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

                    level.bricks.info.emplace_back(components::Brick::Info{
                        .is_solid = false
                    });

                    level.transforms.instances.emplace_back(components::Instance{
                        .size = size,
                        .position = pos,
                        .color = color
                    });
                }
            }
        }

        return level;
    }
} // namespace game::core::gameplay