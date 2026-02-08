//

#include <common.sp>

layout (location=0) in vec3 vColor;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vFragPos;

layout (location=0) out vec4 out_FragColor;

layout (constant_id = 0) const bool isWireframe = false;

void main() {
	// Object base color
	vec3 objectColor = vec3(pc.color);
	float diffuseIntensity = pc.color[3];

	// Light color
	vec3 lightColor = vec3(pc.ambientColor[0], pc.ambientColor[1], pc.ambientColor[2]);

	// Ambient
	float ambientStrength = pc.ambientColor[3];
	vec3 ambientColor = lightColor * ambientStrength;

	// Diffuse
	vec3 normalUnit = normalize(vNormal);
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
	vec4 finalColor = vec4(objectColor, 1.0f) * vec4(ambientColor + diffuseColor + specular, 1.0f);

	out_FragColor = isWireframe ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : finalColor;
};