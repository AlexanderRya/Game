#include <game/core/components/VertexBuffer.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/api/Device.hpp>
#include <game/core/api/Buffer.hpp>

namespace game::core::api {
    vk::Buffer make_buffer(const usize size, const vk::BufferUsageFlags& usage, const api::VulkanContext& ctx) {
        vk::BufferCreateInfo buffer_create_info{}; {
            buffer_create_info.size = size;
            buffer_create_info.queueFamilyIndexCount = 1;
            buffer_create_info.pQueueFamilyIndices = &ctx.device.queue_family;
            buffer_create_info.usage = usage;
            buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
        }

        return ctx.device.logical.createBuffer(buffer_create_info, nullptr, ctx.dispatcher);
    }

    vk::DeviceMemory allocate_memory(const vk::Buffer& buffer, const vk::MemoryPropertyFlags& flags, const api::VulkanContext& ctx) {
        const auto memory_requirements = ctx.device.logical.getBufferMemoryRequirements(buffer, ctx.dispatcher);

        vk::MemoryAllocateInfo memory_allocate_info{}; {
            memory_allocate_info.allocationSize = memory_requirements.size;
            memory_allocate_info.memoryTypeIndex = api::find_memory_type(
                ctx, memory_requirements.memoryTypeBits, flags);
        }

        auto memory = ctx.device.logical.allocateMemory(memory_allocate_info, nullptr, ctx.dispatcher);

        ctx.device.logical.bindBufferMemory(buffer, memory, 0, ctx.dispatcher);

        return memory;
    }

    void copy_buffer(const vk::Buffer& src, const vk::Buffer& dst, const usize size, const VulkanContext& ctx) {
        auto command_buffer = begin_transient(ctx); {
            vk::CommandBufferBeginInfo begin_info{}; {
                begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
            }

            command_buffer.begin(begin_info, ctx.dispatcher);

            vk::BufferCopy region{}; {
                region.size = size;
                region.srcOffset = 0;
                region.dstOffset = 0;
            }

            command_buffer.copyBuffer(src, dst, region, ctx.dispatcher);

            command_buffer.end(ctx.dispatcher);

            end_transient(command_buffer, ctx);
        }
    }
} // namespace game::core::api