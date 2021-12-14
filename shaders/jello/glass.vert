#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vertex;	    // The position of the vertex, in camera space
out vec3 vertexToCamera;    // Vector from the vertex to the eye, which is the camera
out vec3 eyeNormal;	    // Normal of the vertex, in camera space

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;

// Added for SpecularIntensity and Light Information
float shininess = 32.0;
out float SpecularIntensity;

// Light data
const int MAX_LIGHTS = 4;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightColors[MAX_LIGHTS];

uniform bool useLighting;     // Whether to calculate lighting using lighting equation

void main()
{
//    vertex = vec3(v*m*vec4(position, 1.0).x, v*m*vec4(position, 1.0).y, v*m*vec4(position, 1.0).z);
//    vertex = vec3(0.0, 0.0, 0.0);
    vertex = ((v*m)*(vec4(position, 1.0))).xyz;
    eyeNormal = normalize(mat3(transpose(inverse(v*m))) * normal);
//    eyeNormal = normalize(transpose(inverse(v*m)) * vec4(normal, 0.0));
//    eyeNormal = normal;
//      eyeNormal = vec3(0.0, 0.0, 0.0);
    vertexToCamera = -normalize(vertex);
    gl_Position = p*v*m*vec4(position,1.0);

    // Compute SpecularIntensity for use in our jello shader
    vec4 position_cameraSpace = v * m * vec4(position, 1.0);
    vec4 normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);

    if (useLighting) {
        for (int i = 0; i < MAX_LIGHTS; i++) {
            vec4 vertexToLight = vec4(0);
            // Point Light
            if (lightTypes[i] == 0) {
                vertexToLight = normalize(v * vec4(lightPositions[i], 1) - position_cameraSpace);
            } else if (lightTypes[i] == 1) {
                // Dir Light
                vertexToLight = normalize(v * vec4(-lightDirections[i], 0));
            }

            // Add specular component
            vec4 lightReflection = normalize(-reflect(vertexToLight, normal_cameraSpace));
            vec4 eyeDirection = normalize(vec4(0,0,0,1) - position_cameraSpace);
            SpecularIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
        }
    } else {
        SpecularIntensity = 0;
    }
}
