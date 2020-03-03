#include <game/core/components/Mesh.hpp>
#include <game/Constants.hpp>

namespace game::core::components {
    void Mesh::build(const api::VulkanContext& ctx, const api::PipelineLayout& layout) {
        instance_data = {};
        instance_data.resize(1);

        instance_data[0].instances = glm::mat4(1.0f);

        /* Instance descriptor creation*/ {
            api::DescriptorSet::CreateInfo create_info{}; {
                create_info.ctx = &ctx;
                create_info.layout = layout.set;
            }

            descriptor_set.create(create_info);
        }

        /* Instance buffer */ {
            api::MappedBuffer::CreateInfo create_info{}; {
                create_info.ctx = &ctx;
                create_info.type_size = sizeof(Instance);
                create_info.buffer_usage = vk::BufferUsageFlagBits::eStorageBuffer;
            }

            instance_buffer.create(create_info);

            for (usize i = 0; i < meta::frames_in_flight; ++i) {
                instance_buffer[i].write(&instance_data[0], 1);
            }
        }

        /* Instance descriptor update */ {
            api::DescriptorSet::WriteInfo write_info{}; {
                write_info.buffer_info = instance_buffer.get_info();
                write_info.binding = static_cast<u32>(meta::PipelineBinding::Instance);
                write_info.type = vk::DescriptorType::eStorageBuffer;
            }

            descriptor_set.write(write_info);
        }
    }

    std::vector<Vertex> generate_triangle_geometry() {
        return { {
            { { -0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } }
        } };
    }
} // namespace game::core::components