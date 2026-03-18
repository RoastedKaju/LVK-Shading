//

#include <common.sp>

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 out_FragColor;

void main() {
    vec4 color = textureBindless2D(pc.textureId, 0, vUV);

    // Remaps UV from [0, 1] to [-1, 1]
    vec2 centered = vUV * 2.0 - 1.0;

    // Chromatic aberration
    vec2 offset = centered * pc.lightingParams[2];
    float r = textureBindless2D(pc.textureId, 0, vUV + offset).r;
    float g = textureBindless2D(pc.textureId, 0, vUV).g;
    float b = textureBindless2D(pc.textureId, 0, vUV - offset).b;
    color = vec4(r, g, b, 1.0);

    // Simple tonemapping
    color.rgb = color.rgb / (color.rgb + vec3(1.0));

    // Color grading / tint

    // Contrast & brightness

    // Vignette
    float vignette = 1.0 - dot(centered, centered) * pc.lightingParams[1];
    vignette = clamp(vignette, 0.0, 1.0);

    // Film grain
    float noise = fract(sin(dot(vUV, vec2(12.9898, 78.233) + pc.lightingParams[3])) * 43758.5453);
    color.rgb += (noise - 0.5) * 0.08;

    // Gamma correction
    color.rgb = pow(color.rgb, vec3(1.0 / 2.2));

    out_FragColor = vec4(color.rgb * vignette, color.a);
}