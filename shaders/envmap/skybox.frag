#version 330 core

out vec4 fragColor;

uniform samplerCube skybox;

in vec3 pos_object;

void main() {
     fragColor = texture(skybox, pos_object);
//     fragColor = vec4(0.7,0.0,0.7,1.0);
}
