#ifndef GAME_LEVEL_HPP
#define GAME_LEVEL_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <filesystem>

namespace game::core::gameplay {
    [[nodiscard]] std::vector<components::GameObject> load_level(const std::filesystem::path&, const f64, const f64);
} // namespace game::core::gameplay

#endif //GAME_LEVEL_HPP
