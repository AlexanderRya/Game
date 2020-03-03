#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/Constants.hpp>

namespace game::core::api {
    void RenderGraph::build(const VulkanContext& ctx) {
        camera_data = {}; {
            camera_data.pv_matrix = glm::mat4(1.0f);
        }

        /* Camera buffer */ {
            api::MappedBuffer::CreateInfo create_info{}; {
                create_info.ctx = &ctx;
                create_info.type_size = sizeof(CameraData);
                create_info.buffer_usage = vk::BufferUsageFlagBits::eUniformBuffer;
            }

            camera_buffer.create(create_info);

            for (usize i = 0; i < meta::frames_in_flight; ++i) {
                camera_buffer[i].write(&camera_data, 1);
            }
        }

        for (auto& mesh : meshes) {
            mesh.build(ctx, layouts[meta::PipelineLayoutType::MeshGeneric]);
            api::DescriptorSet::WriteInfo write_info{}; {
                write_info.buffer_info = camera_buffer.get_info();
                write_info.binding = static_cast<u32>(meta::PipelineBinding::Camera);
                write_info.type = vk::DescriptorType::eUniformBuffer;
            }

            mesh.descriptor_set.write(write_info);
        }
    }
} // namespace game::core::api