#ifndef GAME_DESCRIPTORSET_HPP
#define GAME_DESCRIPTORSET_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <vulkan/vulkan.hpp>

namespace game::core::api {
    class DescriptorSet {
        std::vector<vk::DescriptorSet> descriptor_sets{};
        const api::VulkanContext* ctx{};
    public:
        struct WriteInfo {
            u64 binding{};
            vk::DescriptorType type{};
            std::vector<vk::DescriptorBufferInfo> buffer_info{};
            vk::DescriptorImageInfo image_info{};
        };

        struct CreateInfo {
            const api::VulkanContext* ctx{};
            vk::DescriptorSetLayout layout{};
        };

        DescriptorSet() = default;

        void create(const CreateInfo&);

        void write(const std::vector<WriteInfo>&);
        void write(const WriteInfo&);

        void write_at(const usize, const WriteInfo&);

        [[nodiscard]] vk::DescriptorSet operator [](const usize) const;
    };
} // namespace game::core::api

#endif //GAME_DESCRIPTORSET_HPP
