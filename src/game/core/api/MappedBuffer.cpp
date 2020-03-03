#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/MappedBuffer.hpp>
#include <game/core/api/Buffer.hpp>
#include <game/Constants.hpp>
#include <game/Logger.hpp>

namespace game::core::api {
    void SingleMappedBuffer::create(const MappedBuffer::CreateInfo& info) {
        ctx = info.ctx;
        buffer_usage = info.buffer_usage;
        type_size = info.type_size;

        // Arbitrary number
        allocate(4);
    }

    void SingleMappedBuffer::allocate(const usize size) {
        current_size = size;

        buffer = api::make_buffer(size * type_size, buffer_usage, *ctx);
        memory = api::allocate_memory(buffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, *ctx);

        mapped = ctx->device.logical.mapMemory(memory, 0, size * type_size, {}, ctx->dispatcher);
    }

    void SingleMappedBuffer::write(const void* data, const usize size) {
        if (size > current_size) {
            deallocate();
            allocate(size);
        }

        std::memcpy(mapped, data, size * type_size);
        current_size = size;
    }

    void SingleMappedBuffer::deallocate() {
        ctx->device.logical.unmapMemory(memory, ctx->dispatcher);

        ctx->device.logical.freeMemory(memory, nullptr, ctx->dispatcher);
        ctx->device.logical.destroyBuffer(buffer, nullptr, ctx->dispatcher);
    }

    vk::DescriptorBufferInfo SingleMappedBuffer::get_info() const {
        vk::DescriptorBufferInfo info{}; {
            info.buffer = buffer;
            info.offset = static_cast<vk::DeviceSize>(0);
            info.range = current_size * type_size;
        }

        return info;
    }

    usize SingleMappedBuffer::size() const {
        return current_size;
    }

    void MappedBuffer::create(const CreateInfo& create_info) {
        mapped_buffers.reserve(meta::frames_in_flight);

        for (usize i = 0; i < meta::frames_in_flight; ++i) {
            mapped_buffers.emplace_back().create(create_info);
        }

        logger::info("Created mapped buffer with size (in bytes): ", create_info.type_size);
    }

    std::vector<vk::DescriptorBufferInfo> MappedBuffer::get_info() const {
        std::vector<vk::DescriptorBufferInfo> info{};
        info.reserve(meta::frames_in_flight);

        for (usize i = 0; i < meta::frames_in_flight; ++i) {
            info.emplace_back(mapped_buffers[i].get_info());
        }

        return info;
    }

    SingleMappedBuffer& MappedBuffer::operator [](const usize idx) {
        return mapped_buffers[idx];
    }
} // namespace game::core::api