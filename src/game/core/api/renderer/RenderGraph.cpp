#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/Texture.hpp>
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
            components::Mesh::BuildInfo info{}; {
                info.ctx = &ctx;
                info.layout = layouts[meta::PipelineLayoutType::MeshGeneric].set;
                info.camera_buffer = &camera_buffer;
                info.texture = &textures[mesh.texture_idx];
            }

            mesh.build(info);
        }
    }
} // namespace game::core::api