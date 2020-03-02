#ifndef GAME_TYPES_HPP
#define GAME_TYPES_HPP

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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
} // namespace game

#endif //GAME_TYPES_HPP
