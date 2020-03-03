#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

namespace game::core {
    class Window {
        GLFWwindow* window{};
    public:
        static inline bool resized = true;
        static inline u32 width{}, height{};

        Window(const i32, const i32, const char*);

        Window(const Window&) = delete;
        Window& operator =(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator =(Window&&) = delete;

        [[nodiscard]] vk::SurfaceKHR create_surface(const api::VulkanContext&) const;

        void poll_events() const;
        void close() const;
        [[nodiscard]] bool should_close() const;
        [[nodiscard]] i32 get_key(const i32) const;
    };
} // namespace game::core

#endif //GAME_WINDOW_HPP
