cmake_minimum_required(VERSION 3.16)
project(Snake)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

include_directories(Snake
        include
        external/glm
        external/glfw/include
        ${Vulkan_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

set(SNAKE_HEADERS
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
        include/game/core/api/renderer/Renderer.hpp include/game/core/components/VertexBuffer.hpp)

set(SNAKE_SOURCES
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
        src/game/core/api/renderer/Renderer.cpp)

add_executable(Snake
        ${SNAKE_HEADERS}
        ${SNAKE_SOURCES})

target_compile_options(Snake PRIVATE -std=c++20 -O3 -pedantic -m64 -Wall -Wextra -Werror -Wno-c99-extensions -Wno-shadow)

if (WIN32)
    target_compile_definitions(Snake PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_definitions(Snake PRIVATE _GLIBCXX_DEBUG)
    endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Snake PRIVATE SNAKE_DEBUG)
endif()

if (UNIX)
    set_target_properties(
            glfw
            PROPERTIES
            COMPILE_OPTIONS -Wno-everything)
endif()

if (WIN32)
    target_link_libraries(Snake glfw)
elseif (UNIX)
    target_link_libraries(Snake glfw pthread)
endif()