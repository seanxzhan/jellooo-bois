#version 330 core

in vec3 position;
in vec3 normal;

out vec3 vertex;	    // The position of the vertex, in camera space
out vec3 vertexToCamera;    // Vector from the vertex to the eye, which is the camera
out vec3 eyeNormal;	    // Normal of the vertex, in camera space

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;

void main()
{
    vertex = ((v*m)*(vec4(position, 1.0))).xyz;
    eyeNormal = normalize(mat3(transpose(inverse(v*m))) * normal);
    vertexToCamera = -normalize(vertex);
    gl_Position = p*v*m*vec4(position,1.0);
}
