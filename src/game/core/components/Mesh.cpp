#include <game/core/components/Texture.hpp>
#include <game/core/components/Mesh.hpp>
#include <game/Constants.hpp>

namespace game::core::components {
    void Mesh::build(const BuildInfo& build_info) {
        instances = {};
        instances.resize(1);

        instances[0].model = glm::mat4(1.0f);

        /* Instance descriptor creation*/ {
            api::DescriptorSet::CreateInfo create_info{}; {
                create_info.ctx = build_info.ctx;
                create_info.layout = build_info.layout;
            }

            descriptor_set.create(create_info);
        }

        /* Instance buffer */ {
            api::MappedBuffer::CreateInfo create_info{}; {
                create_info.ctx = build_info.ctx;
                create_info.type_size = sizeof(Instance);
                create_info.buffer_usage = vk::BufferUsageFlagBits::eStorageBuffer;
            }

            instance_buffer.create(create_info);

            for (usize i = 0; i < meta::frames_in_flight; ++i) {
                instance_buffer[i].write(&instances[0], 1);
            }
        }

        /* Descriptor update */ {
            std::vector<api::DescriptorSet::WriteInfo> write_infos(3); {
                write_infos[0].buffer_info = build_info.camera_buffer->get_info();
                write_infos[0].binding = static_cast<u32>(meta::PipelineBinding::Camera);
                write_infos[0].type = vk::DescriptorType::eUniformBuffer;

                write_infos[1].buffer_info = instance_buffer.get_info();
                write_infos[1].binding = static_cast<u32>(meta::PipelineBinding::Instance);
                write_infos[1].type = vk::DescriptorType::eStorageBuffer;

                write_infos[2].image_info = build_info.texture->get_info();
                write_infos[2].binding = static_cast<u32>(meta::PipelineBinding::DefaultSampler);
                write_infos[2].type = vk::DescriptorType::eCombinedImageSampler;
            }

            descriptor_set.write(write_infos);
        }
    }

    std::vector<Vertex> generate_triangle_geometry() {
        return { {
            { { -0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } }
        } };
    }

    std::vector<Vertex> generate_quad_geometry() {
        return { {
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
            { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
            { { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f } },
            { { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f } },
            { { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f } },
            { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f } },
            { { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } }
        } };
    }
} // namespace game::core::components