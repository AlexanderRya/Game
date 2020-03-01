#ifndef SNAKE_FORWARDS_HPP
#define SNAKE_FORWARDS_HPP

struct GLFWwindow;

namespace vk {
    class Instance;
    class DebugUtilsMessengerEXT;
    class SurfaceKHR;
    class DescriptorPool;
    class CommandPool;
    class CommandBuffer;
    class RenderPass;
    class Framebuffer;
    class Fence;
    class Semaphore;
} // namespace vk

namespace snake::core {
    class Window;
} // namespace snake::core

namespace snake::core::api {
    struct VulkanContext;
    struct Device;
    struct Swapchain;
} // namespace snake::core::api

#endif //SNAKE_FORWARDS_HPP
