#ifndef GAME_RENDERGRAPH_HPP
#define GAME_RENDERGRAPH_HPP

#include <game/core/api/MappedBuffer.hpp>

#include <entt/entt.hpp>
#include <vulkan/vulkan.hpp>

#include <vector>
#include <array>

namespace game::core::api::renderer {
    struct RenderGraph {
        entt::entity main_camera;
        std::vector<entt::entity> objects{};
        std::vector<components::Texture> textures{};

        api::MappedBuffer camera_buffer{};
    };
} // namespace game::core::api::renderer

#endif //GAME_RENDERGRAPH_HPP
