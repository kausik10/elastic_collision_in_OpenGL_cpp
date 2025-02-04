#version 410 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;   // Vertex position
layout(location = 1) in vec3 color;      // Vertex color
layout(location = 2) in vec2 texCoord;   // Texture coordinates

out vec3 fragColor;                      // Pass color to fragment shader
out vec2 TexCoord;                        // Pass texture coordinates

uniform mat4 MVP;                        // Model-View-Projection matrix

void main() {
    gl_Position = MVP * vec4(position, 1.0); // Transform the vertex position
    fragColor = color;                      // Pass color
    TexCoord = texCoord;                     // Pass texture coordinates
}

