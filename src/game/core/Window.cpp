#include <game/core/api/VulkanContext.hpp>
#include <game/core/Globals.hpp>
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

        glfwSetCursorPosCallback(window, [](GLFWwindow*, const double xpos, const double ypos) {
            static double lastX = width / 2.0, lastY = height / 2.0;
            static bool first = true;

            if (first) {
                lastX = xpos;
                lastY = ypos;
                first = false;
            }

            double xoffset = xpos - lastX;
            double yoffset = lastY - ypos;

            lastX = xpos;
            lastY = ypos;

            camera.process(xoffset, yoffset);
        });

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    i32 Window::get_key(const i32 key) const {
        return glfwGetKey(window, key);
    }
} // namespace game::core