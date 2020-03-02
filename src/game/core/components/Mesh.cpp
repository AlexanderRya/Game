#include <game/core/components/Mesh.hpp>

namespace game::core::components {
    std::vector<Vertex> generate_triangle_geometry() {
        return { {
            { { 0.0f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f } }
        } };
    }
} // namespace game::core::components