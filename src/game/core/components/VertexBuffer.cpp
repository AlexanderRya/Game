#include <game/core/components/VertexBuffer.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/Device.hpp>
#include <game/core/api/Buffer.hpp>
#include <game/Logger.hpp>

namespace game::core::components {
    VertexBuffer make_vertex_buffer(const std::vector<Vertex>& vertices, const api::VulkanContext& ctx) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            temp_buffer = api::make_buffer(
                vertices.size() * sizeof(Vertex),
                vk::BufferUsageFlagBits::eTransferSrc,
                ctx);

            temp_memory = api::allocate_memory(
                temp_buffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                ctx);

            auto mapped = ctx.device.logical.mapMemory(temp_memory, 0, vertices.size() * sizeof(Vertex), {}, ctx.dispatcher);
            std::memcpy(mapped, vertices.data(), vertices.size() * sizeof(Vertex));
            ctx.device.logical.unmapMemory(temp_memory, ctx.dispatcher);
        }

        VertexBuffer vertex_buffer;
        /* Allocate device local buffer */ {
            vertex_buffer.handle = make_buffer(
                vertices.size() * sizeof(Vertex),
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                ctx);

            vertex_buffer.memory = api::allocate_memory(vertex_buffer.handle, vk::MemoryPropertyFlagBits::eDeviceLocal, ctx);
        }

        /* Copy to device local */ {
            api::copy_buffer(temp_buffer, vertex_buffer.handle, vertices.size() * sizeof(Vertex), ctx);

            ctx.device.logical.freeMemory(temp_memory, nullptr, ctx.dispatcher);
            ctx.device.logical.destroyBuffer(temp_buffer, nullptr, ctx.dispatcher);
        }

        logger::info("Allocated vertex buffer with size (in bytes): ", vertices.size() * sizeof(Vertex));

        return vertex_buffer;
    }
} // namespace game::core::components