#ifndef GAME_GLOBALS_HPP
#define GAME_GLOBALS_HPP

#include <game/core/components/Camera.hpp>

namespace game::core {
    inline components::Camera camera{};

    inline f32 delta_time = 0;
    inline f32 last_frame = 0;
}

#endif //GAME_GLOBALS_HPP
