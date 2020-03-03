#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

namespace game::meta {
    constexpr static inline usize frames_in_flight = 2;

    enum class PipelineBinding {
        Camera = 0,
        Instance = 1
    };

    enum class PipelineLayoutType {
        MeshGeneric
    };
} // namespace game::meta

#endif //GAME_CONSTANTS_HPP
