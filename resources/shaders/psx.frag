//

#include <common.sp>

layout (location=0) in vec3 vColor;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vFragPos;
layout (location=3) in vec2 vUV;

layout (location=0) out vec4 out_FragColor;

layout (constant_id = 0) const bool isWireframe = false;

void main() {
	
	// PSX has low bit depth (Color Banding)
	vec3 finalColor = vColor.rgb;
	finalColor = floor(finalColor * 31.0f) / 31.0f; // 5-Bit color per Channel

	vec4 diffuseTexture = textureBindless2D(pc.textureId, 0, vUV);
	
	out_FragColor = isWireframe ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : vec4(finalColor, 1.0) * diffuseTexture;
};