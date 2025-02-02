#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec3 fragColor;        // Color passed from vertex shader
out vec4 FragColor;       // Final color output

void main() {
    FragColor = vec4(fragColor, 1.0); // Set the final fragment color
}
