#ifndef GAME_TYPES_HPP
#define GAME_TYPES_HPP

#include <game/Forwards.hpp>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vector>

namespace game {
    using i8 = signed char;
    using i16 = signed short;
    using i32 = signed int;
    using i64 = signed long long;

    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    using f32 = float;
    using f64 = double;

    using isize = i64;
    using usize = u64;

    struct Vertex {
        glm::vec3 pos;
        glm::vec2 tx_coords;
    };

    struct CameraData {
        glm::mat4 pv_matrix;
    };

    struct GameObjectInfo {
        glm::vec2 position, size, velocity;
        glm::vec3 color;
        float rotation;
        bool is_solid;
        bool destroyed;

        void (*update)(GameObjectInfo&);
    };

    struct Instance {
        glm::mat4 model;
    };

    struct alignas(16) ColorInstance {
        glm::vec3 color;
    };
} // namespace game

#endif //GAME_TYPES_HPP
