#version 410 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;   // Input position attribute
layout(location = 1) in vec3 color;      // Input color attribute

out vec3 fragColor;                      // Color passed to fragment shader

uniform mat4 MVP;                        // Model-View-Projection matrix

void main() {
    gl_Position = MVP * vec4(position, 1.0); // Transform the vertex position
    fragColor = color;                    // Pass the color to the fragment shader
}
