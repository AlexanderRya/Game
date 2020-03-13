cmake_minimum_required(VERSION 3.16)
project(Game)

find_package(Vulkan REQUIRED)

add_subdirectory(external/entt)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

include_directories(Game
        include
        external
        external/glm
        external/entt/src
        external/glfw/include
        external/VulkanMemoryAllocator/src
        ${Vulkan_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

set(STB_HEADERS
        external/stb/stb.h
        external/stb/stb_image.h)

set(STB_SOURCES
        src/game/STB_Impl.cpp)

set(VMA_HEADERS
        external/VulkanMemoryAllocator/src/vk_mem_alloc.h)

set(GAME_HEADERS
        ${STB_HEADERS}
        ${VMA_HEADERS}
        include/game/Logger.hpp
        include/game/Util.hpp
        include/game/Types.hpp
        include/game/core/Window.hpp
        include/game/core/api/VulkanContext.hpp
        include/game/Forwards.hpp
        include/game/core/api/Instance.hpp
        include/game/core/api/Device.hpp
        include/game/core/api/Swapchain.hpp
        include/game/core/api/CommandPool.hpp
        include/game/core/api/CommandBuffer.hpp
        include/game/core/api/DescriptorPool.hpp
        include/game/core/api/RenderPass.hpp
        include/game/core/api/Framebuffer.hpp
        include/game/core/api/renderer/Renderer.hpp
        include/game/core/api/VertexBuffer.hpp
        include/game/core/api/Pipeline.hpp
        include/game/core/Game.hpp
        include/game/core/api/Buffer.hpp
        include/game/core/api/DescriptorSet.hpp
        include/game/Constants.hpp
        include/game/core/api/MappedBuffer.hpp
        include/game/core/components/Camera.hpp
        include/game/core/Globals.hpp
        include/game/core/components/Texture.hpp
        include/game/core/api/Image.hpp
        include/game/core/api/Sampler.hpp
        include/game/core/api/renderer/RenderGraph.hpp
        include/game/core/components/GameObject.hpp
        include/game/core/components/Transform.hpp
        include/game/core/components/Brick.hpp
        include/game/core/gameplay/GameLevel.hpp
        include/game/core/components/Player.hpp include/game/core/components/Ball.hpp)

set(GAME_SOURCES
        ${STB_SOURCES}
        src/main.cpp
        src/game/Util.cpp
        src/game/core/Window.cpp
        src/game/core/api/VulkanContext.cpp
        src/game/core/api/Instance.cpp
        src/game/core/api/Device.cpp
        src/game/core/api/Swapchain.cpp
        src/game/core/api/CommandPool.cpp
        src/game/core/api/CommandBuffer.cpp
        src/game/core/api/DescriptorPool.cpp
        src/game/core/api/RenderPass.cpp
        src/game/core/api/Framebuffer.cpp
        src/game/core/api/renderer/Renderer.cpp
        src/game/core/api/Pipeline.cpp
        src/game/core/Game.cpp
        src/game/core/api/VertexBuffer.cpp
        src/game/core/api/Buffer.cpp
        src/game/core/api/DescriptorSet.cpp
        src/game/core/api/MappedBuffer.cpp
        src/game/core/api/VulkanMemoryAllocator.cpp
        src/game/core/components/Camera.cpp
        src/game/core/api/Image.cpp
        src/game/core/components/Texture.cpp
        src/game/core/api/Sampler.cpp
        src/game/core/gameplay/GameLevel.cpp)

add_executable(Game
        ${GAME_HEADERS}
        ${GAME_SOURCES})

if (UNIX)
    target_compile_options(Game PRIVATE -std=c++20 -O0 -pedantic -pedantic-errors -m64 -Wall -Wextra -Werror -Wno-c99-extensions -Wno-shadow)
endif()

if (WIN32)
    target_compile_definitions(Game PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_definitions(Game PRIVATE _GLIBCXX_DEBUG)
    endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Game PRIVATE GAME_DEBUG)
endif()

if (UNIX)
    set_target_properties(
            glfw
            PROPERTIES
            COMPILE_OPTIONS -Wno-everything)
endif()

if (WIN32)
    target_link_libraries(Game glfw)
elseif (UNIX)
    target_link_libraries(Game glfw pthread)
endif()