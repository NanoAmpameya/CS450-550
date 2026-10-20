#version 410 core

in vec3 vColor;
out vec4 fragColor;

void main() {
    // vColor is already interpolated across the triangle by the rasterizer
    fragColor = vec4(vColor, 1.0);
}
