#ifndef GAME_FORWARDS_HPP
#define GAME_FORWARDS_HPP

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

namespace game::core {
    class Window;
} // namespace game::core

namespace game::core::api {
    struct VulkanContext;
    struct Device;
    struct Swapchain;
    struct RenderGraph;
} // namespace game::core::api

namespace game::core::components {
    struct VertexBuffer;
} // namespace game::core::api

#endif //GAME_FORWARDS_HPP
