#ifndef GAME_RENDERGRAPH_HPP
#define GAME_RENDERGRAPH_HPP

#include <game/core/components/Camera.hpp>
#include <game/core/components/Mesh.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/Constants.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

#include <unordered_map>

namespace game::core::api {
    struct RenderGraph {
        std::array<vk::ClearValue, 2> clear_values{};

        CameraData camera_data{};
        api::MappedBuffer camera_buffer{};

        std::unordered_map<meta::PipelineType, api::Pipeline> pipelines;
        std::unordered_map<meta::PipelineLayoutType, api::PipelineLayout> layouts;
        std::unordered_map<meta::SamplerType, vk::Sampler> samplers;

        std::vector<components::Texture> textures;
        std::vector<components::Mesh> meshes;

        void build(const VulkanContext&);
    };
} // namespace game::core::api

#endif //GAME_RENDERGRAPH_HPP
