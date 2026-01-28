#version 430

uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = projection * model * vec4(aPos, 1.0);
}