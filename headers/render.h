// rendering.h
#ifndef RENDERING_H
#define RENDERING_H

#include <GLFW/glfw3.h>

class Rendering {
public:
    // Constructor and setup
    Rendering();

    // Method to render the objects
    void render();

private:
    // Method to initialize OpenGL settings (view, shaders, etc.)
    void initOpenGL();
};

#endif // RENDERING_H
