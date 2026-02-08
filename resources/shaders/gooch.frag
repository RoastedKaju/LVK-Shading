//

#include <common.sp>

layout (location=0) in vec3 vColor;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vFragPos;

layout (location=0) out vec4 out_FragColor;

layout (constant_id = 0) const bool isWireframe = false;

void main() {
	// Object base color
	vec4 objectColor = pc.color;
	// Light color
	vec3 lightColor = vec3(pc.ambientColor[0], pc.ambientColor[1], pc.ambientColor[2]);

	// Ambient
	float ambientStrength = pc.ambientColor[3];
	vec3 ambientColor = lightColor * ambientStrength;

	// Diffuse
	vec3 normalUnit = normalize(vNormal);
	vec3 lightDirection = normalize(vec3(pc.lightPosition) - vFragPos);
	float diffFactor = max(dot(normalUnit, lightDirection), 0.0);
	vec3 diffuseColor = diffFactor * lightColor;

	// Result
	vec4 finalColor = objectColor * vec4(ambientColor + diffuseColor, 1.0f);

	out_FragColor = isWireframe ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : finalColor;
};