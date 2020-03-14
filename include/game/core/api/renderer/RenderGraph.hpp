#ifndef GAME_RENDERGRAPH_HPP
#define GAME_RENDERGRAPH_HPP

#include <game/core/api/MappedBuffer.hpp>

#undef min
#undef max
#include <vulkan/vulkan.hpp>
#include <entt/entt.hpp>

#include <vector>
#include <array>

namespace game::core::api::renderer {
    struct RenderGraph {
        std::vector<components::Texture> textures{};

        api::MappedBuffer camera_buffer{};
    };
} // namespace game::core::api::renderer

#endif //GAME_RENDERGRAPH_HPP
