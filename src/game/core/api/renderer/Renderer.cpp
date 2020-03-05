#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/api/VertexBuffer.hpp>
#include <game/core/components/Mesh.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/core/Globals.hpp>
#include <game/Constants.hpp>

namespace game::core::api {
    Renderer::Renderer(const api::VulkanContext& context)
    : ctx(context) {
        command_buffers = api::make_rendering_command_buffers(ctx);

        vk::SemaphoreCreateInfo semaphore_create_info{};

        image_available.reserve(meta::frames_in_flight);
        render_finished.reserve(meta::frames_in_flight);

        for (u64 i = 0; i < meta::frames_in_flight; ++i) {
            image_available.emplace_back(ctx.device.logical.createSemaphore(semaphore_create_info, nullptr, ctx.dispatcher));
            render_finished.emplace_back(ctx.device.logical.createSemaphore(semaphore_create_info, nullptr, ctx.dispatcher));
        }

        frames_in_flight.resize(meta::frames_in_flight, nullptr);
    }

    void Renderer::init_rendering_data() {
        vertex_buffers[0] = api::make_vertex_buffer(components::generate_triangle_geometry(), ctx);
        vertex_buffers[1] = api::make_vertex_buffer(components::generate_quad_geometry(), ctx);
    }

    void Renderer::acquire_frame() {
        image_index = ctx.device.logical.acquireNextImageKHR(ctx.swapchain.handle, -1, image_available[current_frame], nullptr, ctx.dispatcher).value;

        if (!frames_in_flight[current_frame]) {
            vk::FenceCreateInfo fence_create_info{}; {
                fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
            }

            frames_in_flight[current_frame] = ctx.device.logical.createFence(fence_create_info, nullptr, ctx.dispatcher);
        }

        ctx.device.logical.waitForFences(frames_in_flight[current_frame], true, -1, ctx.dispatcher);
    }

    void Renderer::build(RenderGraph& graph) {
        auto& command_buffer = command_buffers[image_index];
        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        command_buffer.begin(begin_info, ctx.dispatcher);

        vk::ClearValue clear_value{}; {
            clear_value.color = graph.clear_color;
        }

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderArea.extent = ctx.swapchain.extent;
            render_pass_begin_info.framebuffer = ctx.default_framebuffers[image_index];
            render_pass_begin_info.renderPass = ctx.default_render_pass;
            render_pass_begin_info.clearValueCount = 1;
            render_pass_begin_info.pClearValues = &clear_value;
        }

        vk::Viewport viewport{}; {
            viewport.width = ctx.swapchain.extent.width;
            viewport.height = ctx.swapchain.extent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
        }

        vk::Rect2D scissor{}; {
            scissor.extent = ctx.swapchain.extent;
            scissor.offset = { { 0, 0 } };
        }

        command_buffer.setViewport(0, viewport, ctx.dispatcher);
        command_buffer.setScissor(0, scissor, ctx.dispatcher);

        update_camera(graph);
        update_meshes(graph.meshes);

        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline, ctx.dispatcher);

        // Start mesh pass
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graph.pipelines[meta::PipelineType::MeshGeneric].handle, ctx.dispatcher);

        for (auto& mesh : graph.meshes) {
            command_buffer.bindVertexBuffers(0, vertex_buffers[mesh.vertex_buffer_id].buffer.handle, static_cast<vk::DeviceSize>(0), ctx.dispatcher);
            command_buffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                graph.layouts[meta::PipelineLayoutType::MeshGeneric].pipeline,
                0,
                mesh.descriptor_set[current_frame],
                nullptr,
                ctx.dispatcher);
            command_buffer.draw(mesh.vertex_count, mesh.instances.size(), 0, 0, ctx.dispatcher);

            if (mesh.update) { // Debug stuff
                mesh.update(mesh);
            }
        }

        command_buffer.endRenderPass(ctx.dispatcher);

        command_buffer.end(ctx.dispatcher);
    }

    void Renderer::draw() {
        vk::PipelineStageFlags wait_mask{ vk::PipelineStageFlagBits::eColorAttachmentOutput };
        vk::SubmitInfo submit_info{}; {
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffers[image_index];
            submit_info.pWaitDstStageMask = &wait_mask;
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = &image_available[current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &render_finished[current_frame];
        }

        ctx.device.logical.resetFences(frames_in_flight[current_frame], ctx.dispatcher);
        ctx.device.queue.submit(submit_info, frames_in_flight[current_frame], ctx.dispatcher);

        vk::PresentInfoKHR present_info{}; {
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &render_finished[current_frame];
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &ctx.swapchain.handle;
            present_info.pImageIndices = &image_index;
        }

        ctx.device.queue.presentKHR(present_info, ctx.dispatcher);

        ++frames_rendered;
        current_frame = (current_frame + 1) % meta::frames_in_flight;
    }

    void Renderer::update_camera(RenderGraph& graph) {
        auto projection = glm::perspective(
            glm::radians(60.f),
            static_cast<float>(ctx.swapchain.extent.width) / ctx.swapchain.extent.height,
            0.01f, 100.f);
        projection[1][1] *= -1;
        graph.camera_data.pv_matrix = projection * camera.get_view_mat();
        graph.camera_buffer[current_frame].write(&graph.camera_data, 1);
    }

    void Renderer::update_meshes(std::vector<components::Mesh>& meshes) {
        for (auto& mesh : meshes) {
            if (mesh.instance_buffer[current_frame].size() != mesh.instances.size()) {
                mesh.instance_buffer[current_frame].write(mesh.instances.data(), mesh.instances.size());

                api::DescriptorSet::WriteInfo write_info{}; {
                    write_info.binding = static_cast<u32>(meta::PipelineBinding::Instance);
                    write_info.buffer_info = mesh.instance_buffer.get_info();
                    write_info.type = vk::DescriptorType::eStorageBuffer;
                }

                mesh.descriptor_set.write_at(current_frame, write_info);
            } else {
                mesh.instance_buffer[current_frame].write(mesh.instances.data(), mesh.instances.size());
            }
        }
    }
} // namespace game::core::api
