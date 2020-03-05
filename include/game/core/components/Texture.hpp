#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include <game/core/api/Image.hpp>
#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <filesystem>

namespace game::core::components {
    class Texture {
         api::Image image;
         vk::ImageView image_view;
         vk::Sampler sampler;

         const api::VulkanContext& ctx;
    public:
        explicit Texture(const api::VulkanContext&, const vk::Sampler);

        void load(const std::filesystem::path&);

        [[nodiscard]] vk::DescriptorImageInfo get_info() const;
    };
} // namespace game::core::components

#endif //GAME_TEXTURE_HPP
