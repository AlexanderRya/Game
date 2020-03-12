#ifndef GAME_LEVEL_HPP
#define GAME_LEVEL_HPP

#include <game/core/components/Transform.hpp>
#include <game/core/components/Brick.hpp>
#include <game/Types.hpp>

#include <filesystem>

namespace game::core::gameplay {
    struct GameLevel {
        components::Transform solid_bricks_transforms{};
        components::Brick solid_bricks{};

        components::Transform transforms{};
        components::Brick bricks{};
    };

    [[nodiscard]] GameLevel load(const std::filesystem::path&, const f64, const f64);
} // namespace game::core::gameplay

#endif //GAME_LEVEL_HPP
