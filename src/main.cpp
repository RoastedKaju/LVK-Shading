#include <iostream>
#include <memory>
#include <vector>
#include <filesystem>

#include <lvk/LVK.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <lvk/HelpersImGui.h>

#include "shader_processor.h"
#include "sphere_data.h"
#include "model_loader.h"

// Ambient Color
// Directional Light

static bool showWireframe = false;
static bool autoRotateMesh = true;
static float meshColor[3] = { 0.8f, 0.5f, 0.0f };
static float ambientColor[3] = { 1.0f , 1.0f , 1.0f };
static float ambientStrength = 0.1f;
static float lightPosition[3] = { 14.0f, 7.0f, 7.0f };

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

void loadMonkeyModel(
	std::unique_ptr<lvk::IContext>& ctx,
	std::vector<Vertex>& vertData,
	std::vector<uint32_t>& indexData,
	lvk::Holder<lvk::BufferHandle>& vertBufHandle,
	lvk::Holder<lvk::BufferHandle>& IndexBufHandle)
{
	//loadModelData(std::filesystem::absolute(RESOURCE_DIR"/models/monkey.glb"), vertData, indexData);
	loadModelData(std::filesystem::absolute(RESOURCE_DIR"/models/bunny.obj"), vertData, indexData);

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

void setMouseCallbacks(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, [](auto* window, double x, double y) { ImGui::GetIO().MousePos = ImVec2((float)x, (float)y); });
	glfwSetMouseButtonCallback(window, [](auto* window, int button, int action, int mods) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const ImGuiMouseButton_ imguiButton = (button == GLFW_MOUSE_BUTTON_LEFT)
			? ImGuiMouseButton_Left
			: (button == GLFW_MOUSE_BUTTON_RIGHT ? ImGuiMouseButton_Right : ImGuiMouseButton_Middle);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)xpos, (float)ypos);
		io.MouseDown[imguiButton] = action == GLFW_PRESS;
		});
}

void showUI(
	lvk::ImGuiRenderer& imgui,
	lvk::Framebuffer& framebuff,
	lvk::ICommandBuffer& cmdBuff
)
{
	imgui.beginFrame(framebuff);
	ImGui::Begin("Render Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Show Wireframe", &showWireframe);
	ImGui::Checkbox("Auto Rotate Mesh", &autoRotateMesh);
	ImGui::ColorEdit3("Mesh Color", meshColor);
	ImGui::ColorEdit3("Light Color", ambientColor);
	ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
	ImGui::DragFloat3("Light Position", lightPosition);
	ImGui::End();
	imgui.endFrame(cmdBuff);
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

		// UI context
		std::unique_ptr<lvk::ImGuiRenderer> imguiCtx = std::make_unique<lvk::ImGuiRenderer>(*ctx, window, RESOURCE_DIR"/fonts/Terminal.ttf", 13.0f);

		setMouseCallbacks(window);

		// Shaders
		lvk::Holder<lvk::ShaderModuleHandle> vert = loadShaderModule(ctx, std::filesystem::absolute(SHADER_DIR"/gooch.vert"));
		lvk::Holder<lvk::ShaderModuleHandle> frag = loadShaderModule(ctx, std::filesystem::absolute(SHADER_DIR"/gooch.frag"));

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
				},
				{
					.location = 1,
					.format = lvk::VertexFormat::Float3,
					.offset = offsetof(Vertex, normal)
				},
				{
					.location = 2,
					.format = lvk::VertexFormat::Float2,
					.offset = offsetof(Vertex, uv)
				},

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

		// Uniform Buffer
		struct UniformData
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
			glm::vec4 color;
			glm::vec4 ambientColor;
			glm::vec4 lightPosition;
		};

		lvk::Holder<lvk::BufferHandle> uniformBuffer = ctx->createBuffer(
			{ .usage = lvk::BufferUsageBits_Uniform,
			  .storage = lvk::StorageType_Device,
			  .size = sizeof(UniformData),
			  .debugName = "Buffer: per-frame" },
			nullptr);

		// Render Loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glfwGetFramebufferSize(window, &width, &height);

			if (!width || !height)
				continue;

			const float ratio = width / static_cast<float>(height);

			glm::mat4 model = glm::mat4(1.0f);          // identity
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			const float rotationSpeed = autoRotateMesh ? 15.0f : 0.0f;
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			const glm::mat4 v = glm::lookAt(
				glm::vec3(0.0f, 0.15f, 0.35f),   // camera position
				glm::vec3(0.0f, 0.1f, 0.0f),   // look at model
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
			// Uniform version
			UniformData uniformData{};
			uniformData.color = glm::vec4(meshColor[0], meshColor[1], meshColor[2], 1.0f);
			uniformData.model = model;
			uniformData.proj = p;
			uniformData.view = v;
			uniformData.ambientColor = glm::vec4(ambientColor[0], ambientColor[1], ambientColor[2], ambientStrength);
			uniformData.lightPosition = glm::vec4(lightPosition[0], lightPosition[1], lightPosition[2], 1.0f);

			// Command buffer
			lvk::ICommandBuffer& buff = ctx->acquireCommandBuffer();
			buff.cmdUpdateBuffer(uniformBuffer, uniformData);
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
				//buff.cmdPushConstants(pc);
				buff.cmdPushConstants(ctx->gpuAddress(uniformBuffer));
				buff.cmdDrawIndexed((uint32_t)indices.size());

				// Bind Wireframe Pipeline
				if (showWireframe)
				{
					buff.cmdBindRenderPipeline(wireframePipeline);
					buff.cmdSetDepthBiasEnable(true);
					buff.cmdSetDepthBias(0.0f, -1.0f, 0.0f);
					buff.cmdDrawIndexed((uint32_t)indices.size());
				}

				// UI
				showUI(*imguiCtx, framebuffer, buff);
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