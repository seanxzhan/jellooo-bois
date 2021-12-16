#version 330 core

in vec3 vertex;                 // The position of the vertex, in camera space!
in vec3 vertexToCamera;         // Vector from the vertex to the eye, which is the camera
in vec3 eyeNormal;	        // Normal of the vertex, in camera space!

in float SpecularIntensity;

// [NOTE] These are hyperparamters we can experiment with
float r0 = 0.3; // The R0 value to use in Schlick's approximation
vec3  eta = vec3(0.79, 0.8, 0.81);  // Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform mat4 v;
uniform mat4 m;

uniform samplerCube skyBox;
uniform int jelloColor;

out vec4 fragColor;

void main()
{
    vec3 n = normalize(eyeNormal);
    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!
    vec3 vertexToCamera = normalize(vertexToCamera);

    // step 1
    vec4 reflectedDir = vec4(reflect(n, -1 * cameraToVertex), 0.0);
    reflectedDir = inverse(v) * reflectedDir;

    vec4 reflectionColor = texture(skyBox , vec3(reflectedDir));
    reflectionColor.w = 1.0;

    // step 2
    vec4 r_dir = vec4(refract(cameraToVertex, n, eta.r), 0.0);
    vec4 g_dir = vec4(refract(cameraToVertex, n, eta.g), 0.0);
    vec4 b_dir = vec4(refract(cameraToVertex, n, eta.b), 0.0);

    r_dir = inverse(v) * r_dir;
    g_dir = inverse(v) * g_dir;
    b_dir = inverse(v) * b_dir;

    // step 3
    vec4 refractionColor = vec4(texture(skyBox, vec3(r_dir)).r,
                                texture(skyBox, vec3(g_dir)).g,
                                texture(skyBox, vec3(b_dir)).b,
                                1.0);

    // step 4
    float cos_theta = dot(n, -1 * cameraToVertex);
    float F = r0 + ((1 - r0) * pow(1 - cos_theta, 5));

    // step 5
    fragColor = (1-F) * refractionColor + F * reflectionColor;

    // Add Specular Highlight
    fragColor = fragColor + SpecularIntensity;
    fragColor.w = 1.0;

    // Make Jello-Like

    // Keep 1 Channel Color


    if (jelloColor == 0) { // Blue
        float prop = 0.5; // proportion of glass color to keep
        fragColor.x *= 0.1;
        fragColor.y *= 0.1;
        fragColor.z = (1-prop) * 1.0 + prop * fragColor.z;
        fragColor.w = 0.8;
    } else if (jelloColor == 1) { // Red
        float prop = 0.4; // proportion of glass color to keep
        fragColor.z *= 0.1;
        fragColor.y *= 0.1;
        fragColor.x = (1-prop) * 1.0 + prop * fragColor.z;
        fragColor.w = 0.8;
    } else if (jelloColor == 2) { // Green
        float prop = 0.5; // proportion of glass color to keep
        fragColor.x *= 0.2;
        fragColor.z *= 0.2;
        fragColor.y = (1-prop) * 1.0 + prop * fragColor.z;
        fragColor.w = 0.8;
    } else { // white jello ish
//        fragColor.x *= 1.0;
//        fragColor.z *= 1.0;
//        fragColor.y *= 1.0;
//        fragColor.w = 0.1;
        float prop = 0.4; // proportion of glass color to keep
        fragColor.x = (1-prop) * 1.0 + prop * fragColor.x;
        fragColor.z = (1-prop) * 1.0 + prop * fragColor.z;
        fragColor.y = (1-prop) * 1.0 + prop * fragColor.y;
        fragColor.w = 0.8;
    }


//    fragColor = vec4(1.0, 0, 1.0, 1.0);
}
