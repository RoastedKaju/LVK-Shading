//

#include <common.sp>

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inUV;

layout (location=0) out vec3 vColor;
layout (location=1) out vec3 vNormal;
layout (location=2) out vec3 vFragPos;
// For PS1 style texture swimming
layout (location=3) noperspective out vec2 vUV;

layout (constant_id = 0) const bool isWireframe = false;

void main()
{
	// PSX has low precision vertices snap to coarse grid in screen space
	vec4 clip = pc.proj * pc.view * pc.model * vec4(inPos, 1.0);

	const float resScale =  pc.lightingParams.y;
	vec2 targetRes = vec2(320.0, 240.0) * resScale; // Target PS1 resolution
	
	// Clip space -> NDC conversion
	vec2 ndc = clip.xy / clip.w;

	vec2 gridPos = floor(ndc * (targetRes * 0.5) + 0.5);
	vec2 snappedNdc = gridPos / (targetRes * 0.5);

	// Convert snapped NDC back to clip-space
	clip.xy = snappedNdc * clip.w;
	gl_Position = clip;

	vColor = isWireframe ? vec3(0.0f) : inPos.xyz; // will be overridden by Gouraud
	vNormal = mat3(transpose(inverse(pc.model))) * inNormal; 
	vFragPos = vec3(pc.model * vec4(inPos, 1.0f));
	vUV = inUV;

	// In gouraud we calculate the lighting inside vertex shader
	// Object base color
	vec3 objectColor = vec3(pc.color);
	float diffuseIntensity = pc.color[3];

	// Light color
	vec3 lightColor = vec3(pc.ambientColor[0], pc.ambientColor[1], pc.ambientColor[2]);

	// Ambient
	float ambientStrength = pc.ambientColor[3];
	vec3 ambientColor = lightColor * ambientStrength;

	// Diffuse
	vec3 normalUnit = vNormal; // Don't normalize in PSX
	vec3 lightDirection = normalize(vec3(pc.lightPosition) - vFragPos);
	float diffFactor = max(dot(normalUnit, lightDirection), 0.0);
	vec3 diffuseColor = diffFactor * lightColor * diffuseIntensity;

	// Specular
	float specularStrength = pc.lightingParams.x;
	vec3 viewDir = normalize(vec3(pc.cameraPosition) - vFragPos);
	vec3 reflectDir = reflect(-lightDirection, normalUnit);  
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * specFactor * lightColor;

	// Result
	vColor = isWireframe ? vec3(0.0f) : (objectColor) * (ambientColor + diffuseColor + specular);
}