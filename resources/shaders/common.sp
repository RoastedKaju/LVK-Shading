//

layout(std430, buffer_reference) readonly buffer UniformData {
	mat4 model;
	mat4 view;
	mat4 proj;
	vec4 color; // 4th = diffuse Strength
	vec4 ambientColor; // 4th ambient Strength
	vec4 lightPosition;
	vec4 cameraPosition;
	vec4 lightingParams;
	uint textureId;
};

layout(push_constant) uniform PushConstants {
	UniformData pc;
};