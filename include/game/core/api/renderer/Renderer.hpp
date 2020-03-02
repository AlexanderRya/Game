#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <game/core/components/VertexBuffer.hpp>
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

        constexpr static inline auto in_flight = 2;

        const VulkanContext& ctx;

        // Drawing stuff
        std::unordered_map<u64, components::VertexBuffer> vertex_buffers;

        u32 image_index{};
        u32 current_frame{};
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
