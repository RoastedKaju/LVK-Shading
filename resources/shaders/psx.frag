//

#include <common.sp>

layout (location=0) in vec3 vColor;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vFragPos;
layout (location=3) in vec2 vUV;

layout (location=0) out vec4 out_FragColor;

layout (constant_id = 0) const bool isWireframe = false;

// 4x4 Bayer Matrix for ordered dithering
const float ditherTable[16] = float[](
    0.0, 8.0, 2.0, 10.0,
    12.0, 4.0, 14.0, 6.0,
    3.0, 11.0, 1.0, 9.0,
    15.0, 7.0, 13.0, 5.0
);

void main() {
	vec4 diffuseTexture = textureBindless2D(pc.textureId, 0, vUV);
	
	vec3 rawColor = vColor.rgb * diffuseTexture.rgb;

	// Calculate dither offset based on screen-space coordinates
	// divide by higer value to scale the dither pattern (Default: 1/None)
	const int ditherScale = int(pc.lightingParams.z);
	int x = (int(gl_FragCoord.x) / ditherScale) % 4;
	int y = (int(gl_FragCoord.y) / ditherScale) % 4;
	float ditherValue = ditherTable[y * 4 + x];

	// for the color banding to appear increase the divide amount (Default: 32.0)
	const float colorBandingStrength = pc.lightingParams.w;
	float ditherStrength = 1.0 / (32.0 * colorBandingStrength);
	vec3 ditheredColor = rawColor + (ditherValue / 16.0 - 0.5) * ditherStrength;

	// 5-Bit color (PSX standard)
	vec3 finalColor = floor(ditheredColor * 31.0) / 31.0;

	out_FragColor = isWireframe ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(finalColor, diffuseTexture.a);
};