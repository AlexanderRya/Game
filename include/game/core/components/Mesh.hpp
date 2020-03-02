#ifndef GAME_MESH_HPP
#define GAME_MESH_HPP

#include <game/Types.hpp>

#include <vector>

namespace game::core::components {
    struct Mesh {
        u64 vertex_count;
        u64 vertex_buffer_id;
    };

    [[nodiscard]] std::vector<Vertex> generate_triangle_geometry();
}

#endif //GAME_MESH_HPP
