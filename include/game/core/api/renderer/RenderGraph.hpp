#ifndef GAME_RENDERGRAPH_HPP
#define GAME_RENDERGRAPH_HPP

#include <game/core/components/Mesh.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

#include <unordered_map>

namespace game::core::api {
    struct RenderGraph {
        vk::ClearColorValue clear_color{};

        std::unordered_map<u64, api::Pipeline> pipelines;
        std::unordered_map<u64, api::PipelineLayout> pipeline_layouts;

        std::vector<components::Mesh> meshes;
    };
} // namespace game::core::api

#endif //GAME_RENDERGRAPH_HPP
