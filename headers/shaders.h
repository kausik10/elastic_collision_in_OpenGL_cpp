#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>

// Function to load and compile vertex and fragment shaders
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif
