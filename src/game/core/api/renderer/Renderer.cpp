#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/components/GameObject.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/api/VertexBuffer.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/core/Globals.hpp>
#include <game/Constants.hpp>
#include <game/Logger.hpp>

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

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderArea.extent = ctx.swapchain.extent;
            render_pass_begin_info.framebuffer = ctx.default_framebuffers[image_index];
            render_pass_begin_info.renderPass = ctx.default_render_pass;
            render_pass_begin_info.clearValueCount = graph.clear_values.size();
            render_pass_begin_info.pClearValues = graph.clear_values.data();
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

        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline, ctx.dispatcher);

        // Start mesh pass
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graph.pipelines[meta::PipelineType::MeshGeneric].handle, ctx.dispatcher);

        for (auto& object : graph.game_objects) {
            object.instances.resize(object.info.size());
            object.colors.resize(object.info.size());

            for (usize i = 0; i < object.info.size(); ++i) {
                auto& color = object.colors[i].color;
                auto& model = object.instances[i].model;
                auto& info = object.info[i];

                if (info.update) {
                    info.update(info);
                }

                color = info.color;

                model = glm::mat4(1.0f);

                model = glm::translate(model, glm::vec3(info.position, 0.0f));

                model = glm::translate(model, glm::vec3(0.5f * info.size.x, 0.5f * info.size.y, 0.0f));
                model = glm::rotate(model, info.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(-0.5f * info.size.x, -0.5f * info.size.y, 0.0f));

                model = glm::scale(model, glm::vec3(info.size, 1.0f));
            }

            update_objects(object);

            command_buffer.bindVertexBuffers(0, vertex_buffers[object.vertex_buffer_id].buffer.handle, static_cast<vk::DeviceSize>(0), ctx.dispatcher);
            command_buffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                graph.layouts[meta::PipelineLayoutType::MeshGeneric].pipeline,
                0,
                object.descriptor_set[current_frame],
                nullptr,
                ctx.dispatcher);
            command_buffer.draw(object.vertex_count, object.instances.size(), 0, 0, ctx.dispatcher);
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
        /*auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(ctx.swapchain.extent.width) / ctx.swapchain.extent.height,
            0.01f, 100.f);*/

        auto projection =
            glm::ortho(
                0.0f,
                static_cast<float>(ctx.swapchain.extent.width),
                static_cast<float>(ctx.swapchain.extent.height),
                0.0f, -1.0f, 1.0f);

        projection[1][1] *= -1;

        graph.camera_data.pv_matrix = projection;
        graph.camera_buffer[current_frame].write(&graph.camera_data, 1);
    }

    void Renderer::update_objects(components::GameObject& object) {
        auto& instance_buffer = object.instance_buffer[current_frame];
        auto& color_buffer = object.color_buffer[current_frame];

        if (instance_buffer.size() != object.instances.size()) {
            instance_buffer.write(object.instances.data(), object.instances.size());

            api::DescriptorSet::WriteInfo write_info{}; {
                write_info.binding = static_cast<u32>(meta::PipelineBinding::Instance);
                write_info.buffer_info = { instance_buffer.get_info() };
                write_info.type = vk::DescriptorType::eStorageBuffer;
            }

            object.descriptor_set.write_at(current_frame, write_info);
        } else {
            instance_buffer.write(object.instances.data(), object.instances.size());
        }

        if (color_buffer.size() != object.colors.size()) {
            color_buffer.write(object.colors.data(), object.colors.size());

            api::DescriptorSet::WriteInfo write_info{}; {
                write_info.binding = static_cast<u32>(meta::PipelineBinding::Color);
                write_info.buffer_info = { color_buffer.get_info() };
                write_info.type = vk::DescriptorType::eStorageBuffer;
            }

            object.descriptor_set.write_at(current_frame, write_info);
        } else {
            color_buffer.write(object.colors.data(), object.colors.size());
        }
    }
} // namespace game::core::api
