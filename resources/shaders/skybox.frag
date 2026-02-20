//

#include <common.sp>

layout (location=0) in vec3 dir;

layout (location=0) out vec4 out_FragColor;

void main() {
	// Since we are not using the texture Id for meshes we can use it for our cubemap
	out_FragColor = textureBindlessCube(pc.textureId, 0, dir);
};