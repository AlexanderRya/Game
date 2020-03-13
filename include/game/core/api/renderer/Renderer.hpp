#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <game/core/gameplay/GameLevel.hpp>
#include <game/core/api/VertexBuffer.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/Constants.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <unordered_map>
#include <vector>

namespace game::core::api::renderer {
    class Renderer {
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
        std::vector<vk::Fence> frames_in_flight;

        std::vector<vk::CommandBuffer> command_buffers;

        const VulkanContext& ctx;

        // Drawing stuff
        std::vector<api::VertexBuffer> vertex_buffers;

        std::unordered_map<meta::PipelineType, api::Pipeline> pipelines;
        std::unordered_map<meta::PipelineLayoutType, api::PipelineLayout> layouts;
        std::unordered_map<meta::SamplerType, vk::Sampler> samplers;

        u32 frames_rendered{};
        u32 image_index{};
        u32 current_frame{};
    public:
        explicit Renderer(const VulkanContext&);

        // Loading
        void init_rendering_data();
        void build(RenderGraph&, entt::registry&);
        
        // Drawing
        void update_camera(RenderGraph&, entt::registry&);
        void update_object(components::GameObject&, const components::Transform&);

        u32 acquire_frame();
        void start();
        void draw(gameplay::GameLevel&);
        void end();
        void submit();
    };
} // namespace game::core::api::renderer

#endif //GAME_RENDERER_HPP
