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
    class Sampler;
} // namespace vk

namespace game::core {
    class Window;
} // namespace game::core

namespace game::core::api {
    struct VulkanContext;
    struct Device;
    struct Swapchain;
    struct RenderGraph;
    class DescriptorSet;
    struct Image;
} // namespace game::core::api

namespace game::core::api {
    struct VertexBuffer;
    class MappedBuffer;
    class SingleMappedBuffer;
} // namespace game::core::api

namespace game::core::components {
    struct Mesh;
    class Texture;
} // namespace game::core::components

#endif //GAME_FORWARDS_HPP
