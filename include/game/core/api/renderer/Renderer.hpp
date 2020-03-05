#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <game/core/api/VertexBuffer.hpp>
#include <game/Constants.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <unordered_map>
#include <vector>

namespace game::core::api {
    class Renderer {
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
        std::vector<vk::Fence> frames_in_flight;

        std::vector<vk::CommandBuffer> command_buffers;

        const VulkanContext& ctx;

        // Drawing stuff
        std::unordered_map<u64, api::VertexBuffer> vertex_buffers;

        u32 frames_rendered{};
        u32 image_index{};
        u32 current_frame{};

        void update_camera(RenderGraph&);
        void update_meshes(components::Mesh&);
    public:
        explicit Renderer(const VulkanContext&);

        // Loading
        void init_rendering_data();

        // Drawing
        void acquire_frame();
        void build(RenderGraph&);
        void draw();

    };
} // namespace game::core::api

#endif //GAME_RENDERER_HPP
