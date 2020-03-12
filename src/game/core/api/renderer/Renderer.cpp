#include <game/core/api/renderer/RenderGraph.hpp>
#include <game/core/components/GameObject.hpp>
#include <game/core/api/renderer/Renderer.hpp>
#include <game/core/components/Transform.hpp>
#include <game/core/components/Texture.hpp>
#include <game/core/api/VulkanContext.hpp>
#include <game/core/api/CommandBuffer.hpp>
#include <game/core/api/VertexBuffer.hpp>
#include <game/core/api/Pipeline.hpp>
#include <game/core/api/Sampler.hpp>
#include <game/core/Globals.hpp>
#include <game/core/Window.hpp>
#include <game/Constants.hpp>
#include <game/Logger.hpp>

namespace game::core::api::renderer {
    static inline std::vector<Vertex> generate_triangle_geometry() {
        return { {
            { { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.5f } },
            { { 0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f } },
            { { -0.5f, -0.5f, 0.0f }, { -0.5f, -0.5f } }
        } };
    }

    static inline std::vector<Vertex> generate_quad_geometry() {
        return { {
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        } };
    }

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

        layouts[meta::PipelineLayoutType::MeshGeneric] = api::make_generic_pipeline_layout(context);

        samplers[meta::SamplerType::Default] = api::make_default_sampler(context);

        api::Pipeline::CreateInfo create_info{}; {
            create_info.ctx = &context;
            create_info.vertex_path = "../resources/shaders/generic.vert.spv";
            create_info.fragment_path = "../resources/shaders/generic.frag.spv";
            create_info.layout = layouts[meta::PipelineLayoutType::MeshGeneric];
        }

        pipelines[meta::PipelineType::MeshGeneric] = api::make_generic_pipeline(create_info);
    }

    void Renderer::init_rendering_data() {
        vertex_buffers.emplace_back(api::make_vertex_buffer(generate_triangle_geometry(), ctx));
        vertex_buffers.emplace_back(api::make_vertex_buffer(generate_quad_geometry(), ctx));
    }

    void Renderer::build(RenderGraph& graph, entt::registry& registry) {
        /* Camera buffer */ {
            api::MappedBuffer::CreateInfo create_info{}; {
                create_info.ctx = &ctx;
                create_info.buffer_usage = vk::BufferUsageFlagBits::eUniformBuffer;
                create_info.type_size = sizeof(components::CameraData);
            }

            graph.camera_buffer.create(create_info);
        }

        auto view = registry.view<components::GameObject, components::Texture>();

        for (auto& each : view) {
            auto [object, texture] = view.get<components::GameObject, components::Texture>(each);

            api::DescriptorSet::CreateInfo descriptor_set_info{}; {
                descriptor_set_info.ctx = &ctx;
                descriptor_set_info.layout = layouts[meta::PipelineLayoutType::MeshGeneric].set;
            }

            object.descriptor_set.create(descriptor_set_info);

            api::MappedBuffer::CreateInfo buffer_info{}; {
                buffer_info.ctx = &ctx;
                buffer_info.buffer_usage = vk::BufferUsageFlagBits::eStorageBuffer;
                buffer_info.type_size = sizeof(components::ModelColor);
            }

            object.instance_buffer.create(buffer_info);

            std::vector<api::DescriptorSet::WriteInfo> write_info(3); {
                write_info[0].buffer_info = graph.camera_buffer.get_info();
                write_info[0].binding = static_cast<u32>(meta::PipelineBinding::Camera);
                write_info[0].type = vk::DescriptorType::eUniformBuffer;

                write_info[1].buffer_info = object.instance_buffer.get_info();
                write_info[1].binding = static_cast<u32>(meta::PipelineBinding::Instance);
                write_info[1].type = vk::DescriptorType::eStorageBuffer;

                write_info[2].image_info = texture.get_info(samplers[meta::SamplerType::Default]);
                write_info[2].binding = static_cast<u32>(meta::PipelineBinding::DefaultSampler);
                write_info[2].type = vk::DescriptorType::eCombinedImageSampler;
            }

            object.descriptor_set.write(write_info);
        }
    }

    void Renderer::update_camera(RenderGraph& graph, entt::registry& registry) {
        auto temp_view = registry.view<components::CameraData>();
        auto& camera_data = temp_view.get<components::CameraData>(temp_view[0]);

        camera_data.pvmat =
            glm::ortho(
                0.0f,
                static_cast<float>(ctx.swapchain.extent.width),
                static_cast<float>(ctx.swapchain.extent.height),
                0.0f,
                -1.0f, 1.0f);

        camera_data.pvmat[1][1] *= -1;

        graph.camera_buffer[current_frame].write(&camera_data, 1);
    }

    void Renderer::update_object(components::GameObject& object, const components::Transform& transform) {
        std::vector<components::ModelColor> instances{};
        instances.reserve(transform.instances.size());

        for (const auto& instance : transform.instances) {
            auto& model_color = instances.emplace_back(); {
                model_color.model = glm::mat4(1.0f);
                model_color.model = glm::translate(model_color.model, instance.position);
                model_color.model = glm::scale(model_color.model, instance.size);
                model_color.model = glm::rotate(model_color.model, instance.rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });

                model_color.color = instance.color;
            }
        }

        auto& current_buffer = object.instance_buffer[current_frame];

        if (current_buffer.size() != instances.size()) {
            current_buffer.write(instances.data(), instances.size());

            api::DescriptorSet::WriteInfo write_info{}; {
                write_info.binding = static_cast<u32>(meta::PipelineBinding::Instance);
                write_info.type = vk::DescriptorType::eStorageBuffer;
                write_info.buffer_info = { current_buffer.get_info() };
            }

            object.descriptor_set.write_at(current_frame, write_info);
        } else {
            current_buffer.write(instances.data(), instances.size());
        }
    }

    u32 Renderer::acquire_frame() {
        image_index = ctx.device.logical.acquireNextImageKHR(ctx.swapchain.handle, -1, image_available[current_frame], nullptr, ctx.dispatcher).value;

        if (!frames_in_flight[current_frame]) {
            vk::FenceCreateInfo fence_create_info{}; {
                fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
            }

            frames_in_flight[current_frame] = ctx.device.logical.createFence(fence_create_info, nullptr, ctx.dispatcher);
        }

        ctx.device.logical.waitForFences(frames_in_flight[current_frame], true, -1, ctx.dispatcher);

        return current_frame;
    }

    void Renderer::start() {
        auto& command_buffer = command_buffers[image_index];

        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        command_buffer.begin(begin_info, ctx.dispatcher);

        std::array<vk::ClearValue, 1> clear_values{}; {
            clear_values[0].color = { std::array { 0.01f, 0.01f, 0.01f, 0.01f } };

            // clear_values[1].depthStencil = { { 1.0f, 0 } };
        }

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderArea.extent = ctx.swapchain.extent;
            render_pass_begin_info.framebuffer = ctx.default_framebuffers[image_index];
            render_pass_begin_info.renderPass = ctx.default_render_pass;
            render_pass_begin_info.clearValueCount = clear_values.size();
            render_pass_begin_info.pClearValues = clear_values.data();
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

        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline, ctx.dispatcher);
    }

    void Renderer::draw(RenderGraph& graph, entt::registry& registry) {
        auto objects_view = registry.view<components::GameObject, components::Transform, components::Texture>();
        auto& command_buffer = command_buffers[image_index];

        update_camera(graph, registry);

        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines[meta::PipelineType::MeshGeneric].handle, ctx.dispatcher);

        for (auto it = objects_view.end(); it != objects_view.begin();) {
            auto [object, transform, texture] = objects_view.get<components::GameObject, components::Transform, components::Texture>(*--it);
            update_object(object, transform);

            command_buffer.bindVertexBuffers(0, vertex_buffers[object.vertex_buffer_idx].buffer.handle, static_cast<vk::DeviceSize>(0), ctx.dispatcher);
            command_buffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                layouts[meta::PipelineLayoutType::MeshGeneric].pipeline,
                0, object.descriptor_set[current_frame],
                nullptr, ctx.dispatcher);
            command_buffer.draw(object.vertex_count, transform.instances.size(), 0, 0, ctx.dispatcher);
        }
    }

    void Renderer::end() {
        auto& command_buffer = command_buffers[image_index];

        command_buffer.endRenderPass(ctx.dispatcher);

        command_buffer.end(ctx.dispatcher);
    }

    void Renderer::submit() {
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
} // namespace game::core::api::renderer
