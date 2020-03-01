#include <game/core/api/VulkanContext.hpp>
#include <game/core/Window.hpp>

using namespace snake::core;

int main() {
    Window window(1280, 720, "Snake");
    (void)api::make_vulkan_context(&window);
    return 0;
}
