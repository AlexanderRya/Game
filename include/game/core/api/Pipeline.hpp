#ifndef GAME_PIPELINE_HPP
#define GAME_PIPELINE_HPP

#include <game/Forwards.hpp>

#include <filesystem>

#include <vulkan/vulkan.hpp>

namespace game::core::api {
    struct PipelineLayout {
        vk::PipelineLayout pipeline{};
        vk::DescriptorSetLayout descriptor_set{};
    };

    struct Pipeline {
        struct CreateInfo {
            const VulkanContext* ctx;

            std::filesystem::path vertex_path{};
            std::filesystem::path fragment_path{};

            PipelineLayout layout{};
        };

        PipelineLayout layout{};

        vk::Pipeline handle{};
    };

    [[nodiscard]] PipelineLayout make_generic_pipeline_layout(const VulkanContext&);
    [[nodiscard]] Pipeline make_generic_pipeline(const Pipeline::CreateInfo&);
} // namespace game::core::api

#endif //GAME_PIPELINE_HPP
