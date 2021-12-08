#version 400 core

const float scale = 500;

layout (location = 0) in vec3 aPos;

out vec3 pos_object;

uniform mat4 view;
uniform mat4 projection;

void main() {
     pos_object = aPos;
     gl_Position = projection * view * vec4(aPos * scale, 1);
}
