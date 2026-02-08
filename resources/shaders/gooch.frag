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
	
	// Gooch parameters
	float alpha = 0.25;  // Cool color blend
	float beta = 0.5;    // Warm color blend
	vec3 coolTint = vec3(0.0, 0.0, 0.55);   // Blue tint
	vec3 warmTint = vec3(0.3, 0.3, 0.0);    // Yellow tint
	
	// Calculate lighting direction
	vec3 normalUnit = normalize(vNormal);
	vec3 lightDirection = normalize(vec3(pc.lightPosition) - vFragPos);
	
	// Gooch diffuse term: remap from [-1, 1] to [0, 1]
	float goochDiffuse = (1.0 + dot(lightDirection, normalUnit)) * 0.5;
	
	// Cool and warm colors
	vec3 kCool = coolTint + alpha * objectColor.rgb;
	vec3 kWarm = warmTint + beta * objectColor.rgb;
	
	// Blend between cool and warm based on lighting
	vec3 gooch = (goochDiffuse * kWarm) + ((1.0 - goochDiffuse) * kCool);
	
	// Apply light color
	vec3 finalColor = gooch * lightColor;
	
	out_FragColor = isWireframe ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(finalColor, objectColor.a);
};