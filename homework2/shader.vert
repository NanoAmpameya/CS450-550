#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in float aTriId;

uniform float uTime;

out vec3 vColor;

void main() {
    vec2 p = aPos.xy;

    // Triangle 0 (sail): rotate around the mast base so it "flaps"
    // Triangle 1 (hull): bob on a wave with a different frequency
    if (aTriId < 0.5) {
        vec2 pivot = vec2(0.0, -0.05);
        vec2 d = p - pivot;
        float angle = 0.28 * sin(uTime * 2.0);
        mat2 R = mat2(cos(angle), -sin(angle),
                      sin(angle),  cos(angle));
        p = R * d + pivot;
    } else {
        p.y += 0.07 * sin(uTime * 1.6);
        p.x += 0.03 * sin(uTime * 0.9);
    }

    gl_Position = vec4(p, aPos.z, 1.0);

    // Color from vertex position (then the rasterizer interpolates vColor)
    vColor = vec3(aPos.x * 0.5 + 0.5,
                  aPos.y * 0.5 + 0.5,
                  0.55);
}
