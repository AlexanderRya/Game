#ifndef SNAKE_RENDERER_HPP
#define SNAKE_RENDERER_HPP

#include <game/Forwards.hpp>

#include <vector>

namespace snake::core::api {
    class Renderer {
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
        std::vector<vk::Fence> frames_in_flight;

        const VulkanContext& ctx;
    public:
        explicit Renderer(const VulkanContext&);
    };
} // namespace snake::core::api

#endif //SNAKE_RENDERER_HPP
