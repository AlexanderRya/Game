#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

namespace game::meta {
    constexpr static inline usize frames_in_flight = 2;

    enum class PipelineBinding : u32 {
        Camera = 0,
        Instance = 1,
        DefaultSampler = 2
    };

    enum class PipelineType {
        MeshGeneric
    };

    enum class PipelineLayoutType {
        MeshGeneric
    };

    enum class SamplerType {
        Default
    };
} // namespace game::meta

#endif //GAME_CONSTANTS_HPP
