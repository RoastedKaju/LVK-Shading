//

#include <common.sp>

layout (location=0) in vec3 vColor;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vFragPos;
layout (location=3) in vec2 vUV;

layout (location=0) out vec4 out_FragColor;

layout (constant_id = 0) const bool isWireframe = false;

void main() {
	out_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black outline
	return;
}