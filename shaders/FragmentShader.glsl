#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec3 fragColor;        // Interpolated color from vertex shader
in vec2 TexCoord;         // Interpolated texture coordinates

out vec4 FragColor;       // Final output color

uniform sampler2D texture1; // Texture sampler

void main() {
    vec4 texColor = texture(texture1, TexCoord); // Sample texture
    FragColor = texColor; // Use texture color (or mix it with fragColor)
}

