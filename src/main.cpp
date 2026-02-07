#include <iostream>
#include <memory>
#include <vector>
#include <filesystem>

#include <lvk/LVK.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shader_processor.h"
#include "sphere_data.h"
#include "model_loader.h"

void generateSphereBuffers(
	std::unique_ptr<lvk::IContext>& ctx,
	std::vector<Vertex>& vertData,
	std::vector<uint32_t>& indexData,
	lvk::Holder<lvk::BufferHandle>& vertBufHandle,
	lvk::Holder<lvk::BufferHandle>& IndexBufHandle)
{
	// Generate UV sphere
	generateUVSphere(1.0f, 32, 64, vertData, indexData);
	// Vertex buffer
	lvk::BufferDesc vertBufDesc{};
	vertBufDesc.usage = lvk::BufferUsageBits_Vertex;
	vertBufDesc.storage = lvk::StorageType_Device;
	vertBufDesc.size = sizeof(Vertex) * vertData.size();
	vertBufDesc.data = vertData.data();
	vertBufDesc.debugName = "Buffer: vertex";
	vertBufHandle = ctx->createBuffer(vertBufDesc);
	// Index Buffer
	lvk::BufferDesc indexBufDes{};
	indexBufDes.usage = lvk::BufferUsageBits_Index;
	indexBufDes.storage = lvk::StorageType_Device;
	indexBufDes.size = sizeof(uint32_t) * indexData.size();
	indexBufDes.data = indexData.data();
	indexBufDes.debugName = "Buffer: index";
	IndexBufHandle = ctx->createBuffer(indexBufDes);
}

void loadMonkeyModel (
	std::unique_ptr<lvk::IContext>& ctx,
	std::vector<Vertex>& vertData,
	std::vector<uint32_t>& indexData,
	lvk::Holder<lvk::BufferHandle>& vertBufHandle,
	lvk::Holder<lvk::BufferHandle>& IndexBufHandle)
{
	loadModelData(std::filesystem::absolute(RESOURCE_DIR"/models/monkey.glb"), vertData, indexData);

	// Vertex buffer
	lvk::BufferDesc vertBufDesc{};
	vertBufDesc.usage = lvk::BufferUsageBits_Vertex;
	vertBufDesc.storage = lvk::StorageType_Device;
	vertBufDesc.size = sizeof(Vertex) * vertData.size();
	vertBufDesc.data = vertData.data();
	vertBufDesc.debugName = "Buffer: vertex";
	vertBufHandle = ctx->createBuffer(vertBufDesc);
	// Index Buffer
	lvk::BufferDesc indexBufDes{};
	indexBufDes.usage = lvk::BufferUsageBits_Index;
	indexBufDes.storage = lvk::StorageType_Device;
	indexBufDes.size = sizeof(uint32_t) * indexData.size();
	indexBufDes.data = indexData.data();
	indexBufDes.debugName = "Buffer: index";
	IndexBufHandle = ctx->createBuffer(indexBufDes);
}

int main()
{
	minilog::LogConfig configInfo{};
	configInfo.threadNames = false;
	minilog::initialize(nullptr, configInfo);

	int width = -95;
	int height = -90;

	GLFWwindow* window = lvk::initWindow("Shading", width, height, false);

	{
		// Context
		std::unique_ptr<lvk::IContext> ctx = lvk::createVulkanContextWithSwapchain(window, width, height, {});

		// Shaders
		lvk::Holder<lvk::ShaderModuleHandle> vert = loadShaderModule(ctx, std::filesystem::absolute(SHADER_DIR"/main.vert"));
		lvk::Holder<lvk::ShaderModuleHandle> frag = loadShaderModule(ctx, std::filesystem::absolute(SHADER_DIR"/main.frag"));

		// Depth texture
		lvk::TextureDesc depthTextureDesc{};
		depthTextureDesc.type = lvk::TextureType_2D;
		depthTextureDesc.format = lvk::Format_Z_F32;
		depthTextureDesc.dimensions = { (uint32_t)width, (uint32_t)height };
		depthTextureDesc.usage = lvk::TextureUsageBits_Attachment;
		depthTextureDesc.debugName = "Depth Buffer";
		lvk::Holder<lvk::TextureHandle> depthTexture = ctx->createTexture(depthTextureDesc);

		// Buffers
		// Sphere Data and Buffers
		std::vector<Vertex> verts;
		std::vector<uint32_t> indices;
		lvk::Holder<lvk::BufferHandle> vertexBuffer;
		lvk::Holder<lvk::BufferHandle> indexBuffer;

		//generateSphereBuffers(ctx, verts, indices, vertexBuffer, indexBuffer);
		loadMonkeyModel(ctx, verts, indices, vertexBuffer, indexBuffer);

		// Attributes
		const lvk::VertexInput vdesc = {
			.attributes = {
				{
					.location = 0,
					.format = lvk::VertexFormat::Float3,
					.offset = offsetof(Vertex, position)
				}
			},
			.inputBindings = { {.stride = sizeof(Vertex) } }
		};


		// Solid pipeline
		lvk::RenderPipelineDesc pipelineDesc{};
		pipelineDesc.vertexInput = vdesc;
		pipelineDesc.smVert = vert;
		pipelineDesc.smFrag = frag;
		pipelineDesc.color[0].format = ctx->getSwapchainFormat();
		pipelineDesc.depthFormat = ctx->getFormat(depthTexture);

		lvk::Holder<lvk::RenderPipelineHandle> soildPipeline = ctx->createRenderPipeline(pipelineDesc);

		// Wireframe pipeline
		uint32_t isWireframe = 1;
		lvk::SpecializationConstantEntry wireframeSpecInfoEntry{};
		wireframeSpecInfoEntry.constantId = 0;
		wireframeSpecInfoEntry.size = sizeof(uint32_t);

		lvk::RenderPipelineDesc wireframePipelineDesc{};
		wireframePipelineDesc.vertexInput = vdesc;
		wireframePipelineDesc.smVert = vert;
		wireframePipelineDesc.smFrag = frag;
		wireframePipelineDesc.color[0].format = ctx->getSwapchainFormat();
		wireframePipelineDesc.depthFormat = ctx->getFormat(depthTexture);
		wireframePipelineDesc.polygonMode = lvk::PolygonMode_Line;
		wireframePipelineDesc.specInfo.entries[0] = wireframeSpecInfoEntry;
		wireframePipelineDesc.specInfo.data = &isWireframe;
		wireframePipelineDesc.specInfo.dataSize = sizeof(isWireframe);

		lvk::Holder<lvk::RenderPipelineHandle> wireframePipeline = ctx->createRenderPipeline(wireframePipelineDesc);

		LVK_ASSERT(soildPipeline.valid());
		LVK_ASSERT(wireframePipeline.valid());

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glfwGetFramebufferSize(window, &width, &height);

			if (!width || !height)
				continue;

			const float ratio = width / static_cast<float>(height);

			glm::mat4 model = glm::mat4(1.0f);          // identity
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			const glm::mat4 v = glm::lookAt(
				glm::vec3(0.0f, 1.0f, 3.0f),   // camera position
				glm::vec3(0.0f, 0.0f, 0.0f),   // look at model
				glm::vec3(0.0f, 1.0f, 0.0f)    // up direction
			);
			const glm::mat4 p = glm::perspective(45.0f, ratio, 0.1f, 1000.0f);

			lvk::RenderPass renderPass;
			renderPass.color[0].loadOp = lvk::LoadOp_Clear;
			renderPass.color[0].clearColor.float32[0] = 0.5f;
			renderPass.color[0].clearColor.float32[1] = 0.5f;
			renderPass.color[0].clearColor.float32[2] = 0.5f;
			renderPass.color[0].clearColor.float32[3] = 1.0f;
			renderPass.depth.loadOp = lvk::LoadOp_Clear; // Depth
			renderPass.depth.clearDepth = 1.0f;

			// Frame buffer
			lvk::Framebuffer framebuffer;
			framebuffer.color[0].texture = ctx->getCurrentSwapchainTexture();
			framebuffer.depthStencil.texture = depthTexture;

			// Perframe data
			const struct PerFrameData
			{
				glm::mat4 mvp;
			} pc = { .mvp = p * v * model };

			// Command buffer
			lvk::ICommandBuffer& buff = ctx->acquireCommandBuffer();
			// Begin Rendering
			buff.cmdBeginRendering(renderPass, framebuffer);
			buff.cmdPushDebugGroupLabel("Render Triangle", 0xff0000ff);
			{
				// Bindings
				buff.cmdBindVertexBuffer(0, vertexBuffer);
				buff.cmdBindIndexBuffer(indexBuffer, lvk::IndexFormat_UI32);
				// Bind solid pipeline
				buff.cmdBindRenderPipeline(soildPipeline);
				buff.cmdBindDepthState({ .compareOp = lvk::CompareOp_Less, .isDepthWriteEnabled = true });
				buff.cmdPushConstants(pc);
				buff.cmdDrawIndexed((uint32_t)indices.size());

				// Bind Wireframe Pipeline
				buff.cmdBindRenderPipeline(wireframePipeline);
				buff.cmdSetDepthBiasEnable(true);
				buff.cmdSetDepthBias(0.0f, -1.0f, 0.0f);
				buff.cmdDrawIndexed((uint32_t)indices.size());
			}
			buff.cmdPopDebugGroupLabel();
			buff.cmdEndRendering();

			// Submission
			ctx->submit(buff, ctx->getCurrentSwapchainTexture());
		}

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}