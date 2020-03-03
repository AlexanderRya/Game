#include <game/core/api/MappedBuffer.hpp>
#include <game/core/api/DescriptorSet.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/Constants.hpp>

namespace game::core::api {
    void DescriptorSet::create(const CreateInfo& info) {
        ctx = info.ctx;

        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.descriptorSetCount = 1;
            allocate_info.pSetLayouts = &info.layout;
            allocate_info.descriptorPool = ctx->descriptor_pool;
        }
        descriptor_sets.resize(meta::frames_in_flight);

        for (usize i = 0; i < meta::frames_in_flight; ++i) {
            descriptor_sets[i] = ctx->device.logical.allocateDescriptorSets(allocate_info, ctx->dispatcher).back();
        }
    }

    void DescriptorSet::write(const std::vector<WriteInfo>& write_info) {
        std::vector<vk::WriteDescriptorSet> writes;
        writes.resize(write_info.size());

        for (usize j = 0; j < meta::frames_in_flight; ++j) {
            for (usize i = 0; i < write_info.size(); ++i) {
                writes[i].descriptorType = write_info[i].type;
                writes[i].dstBinding = write_info[i].binding;
                writes[i].descriptorCount = 1;
                writes[i].dstArrayElement = 0;
                writes[i].dstSet = descriptor_sets[j];
                writes[i].pImageInfo = nullptr;
                writes[i].pTexelBufferView = nullptr;
                writes[i].pBufferInfo = &write_info[i].buffer_info[j];

                ctx->device.logical.updateDescriptorSets(writes, nullptr, ctx->dispatcher);
            }
        }
    }

    void DescriptorSet::write(const WriteInfo& write_info) {
        for (usize i = 0; i < meta::frames_in_flight; ++i) {
            vk::WriteDescriptorSet write{}; {
                write.descriptorType = write_info.type;
                write.dstBinding = write_info.binding;
                write.descriptorCount = 1;
                write.dstArrayElement = 0;
                write.dstSet = descriptor_sets[i];
                write.pImageInfo = nullptr;
                write.pTexelBufferView = nullptr;
                write.pBufferInfo = &write_info.buffer_info[i];
            }

            ctx->device.logical.updateDescriptorSets(write, nullptr, ctx->dispatcher);
        }
    }

    vk::DescriptorSet DescriptorSet::operator [](const usize idx) const {
        return descriptor_sets[idx];
    }
} // namespace game::core::api