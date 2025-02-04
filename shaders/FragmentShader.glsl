#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec3 fragColor;        
in vec2 TexCoord;         

out vec4 FragColor;       

uniform sampler2D textureSampler; 
uniform bool isGround;  // New uniform to check if it's the ground

void main() {
    if (isGround) {
        vec4 texColor = texture(textureSampler, TexCoord); 
    } else {
        vec4 texColor = texture(textureSampler, TexCoord); 
        FragColor = texColor;
    }
}

