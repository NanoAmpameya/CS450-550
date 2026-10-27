#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec3 vColor;

void main() {
    gl_Position = proj_matrix * mv_matrix * vec4(aPos, 1.0);
    vColor = aColor;
}
