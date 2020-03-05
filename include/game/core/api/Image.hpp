#ifndef GAME_IMAGE_HPP
#define GAME_IMAGE_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <filesystem>

namespace game::core::api {
    struct Image {
        struct CreateInfo {
            i32 width{};
            i32 height{};

            vk::Format format{};
            vk::ImageTiling tiling{};

            vk::ImageUsageFlags usage_flags{};
            VmaMemoryUsage memory_usage{};

            const VulkanContext* ctx;
        };

        vk::Image handle{};
        VmaAllocation allocation{};
    };

    [[nodiscard]] Image make_image(const Image::CreateInfo&);
    [[nodiscard]] vk::ImageView make_image_view(const VulkanContext&, const vk::Image, const vk::Format, const vk::ImageAspectFlags);
    void transition_image_layout(const VulkanContext&, vk::Image, const vk::ImageLayout, const vk::ImageLayout);
} // namespace game::core::api

#endif //GAME_IMAGE_HPP
