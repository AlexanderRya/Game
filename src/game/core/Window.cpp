#include <game/core/api/VulkanContext.hpp>
#include <game/core/Window.hpp>
#include <game/Logger.hpp>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace game::core {
    Window::Window(const i32 w, const i32 h, const char* title) {
        glfwSetErrorCallback([](const i32 code, const char* message) {
            std::cout << util::format(
                "[{}] [GLFW3] [Error: {}]: {}\n",
                util::get_current_timestamp(),
                code,
                message);
        });

        if (!glfwInit()) {
            throw std::runtime_error("Failed glfw init");
        }

        logger::info("glfwInit() success");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        width = w;
        height = h;

        if (!(window = glfwCreateWindow(w, h, title, nullptr, nullptr))) {
            throw std::runtime_error("Failed window creation");
        }

        logger::info("Window successfully created with size: ", width, "x", height);
    }

    vk::SurfaceKHR Window::create_surface(const api::VulkanContext& ctx) const {
        VkSurfaceKHR surface;

        glfwCreateWindowSurface(static_cast<VkInstance>(ctx.instance), window, nullptr, &surface);

        return surface;
    }

    void Window::poll_events() const {
        glfwPollEvents();
    }

    bool Window::should_close() const {
        return glfwWindowShouldClose(window);
    }

    void Window::close() const {
        glfwSetWindowShouldClose(window, true);
    }
} // namespace game::core