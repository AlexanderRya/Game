#ifndef GAME_GAMELEVEL_HPP
#define GAME_GAMELEVEL_HPP

#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/Transform.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/components/Brick.hpp>
#include <game/Types.hpp>

#include <entt/entt.hpp>

#include <filesystem>

namespace game::core::gameplay {
    struct GameLevel {
        entt::registry registry;
        api::renderer::RenderGraph graph;

        const api::VulkanContext& ctx;

        explicit GameLevel(const api::VulkanContext& ctx) : ctx(ctx) {}

        void load(const std::filesystem::path&, const f64, const f64);
    };
} // namespace game::core::gameplay

#endif //GAME_GAMELEVEL_HPP
