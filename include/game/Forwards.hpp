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
    class DescriptorSet;
    struct Image;
    struct VertexBuffer;
    class MappedBuffer;
    class SingleMappedBuffer;
} // namespace game::core::api

namespace game::core::components {
    struct GameObject;
    struct Transform;
    class Texture;
} // namespace game::core::components

namespace game::core::api::renderer {
    struct RenderGraph;
} // namespace game::core::api::renderer

namespace game::core::api::imgui {
    struct ImGuiContext;
} // namespace game::core::api::imgui

#endif //GAME_FORWARDS_HPP
