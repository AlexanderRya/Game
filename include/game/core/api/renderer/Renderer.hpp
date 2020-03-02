#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <unordered_map>
#include <vector>

namespace game::core::api {
    class Renderer {
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
        std::vector<vk::Fence> frames_in_flight;

        const VulkanContext& ctx;
    public:
        explicit Renderer(const VulkanContext&);
    };
} // namespace game::core::api

#endif //GAME_RENDERER_HPP
