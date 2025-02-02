#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Sphere {
public:
    Sphere(float radius, int sectors, int stacks);
    ~Sphere();

    bool checkCollision(const Sphere& otherSphere);
    void generateSphere();
    void draw(GLuint programID, GLuint MatrixID, const glm::mat4& MVP);
    void update(float deltaTime, float velocity);  // Pass velocity during update
    void updatePosition(glm::vec3 position);

private:
    float radius;
    int sectors;
    int stacks;
    GLuint vertexArrayID;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    GLuint indexBuffer;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;

    glm::vec3 position;  // Position of the sphere
    float angle;  // Angle of rotation around the y-axis

    void generateVertices();
    void generateColors();
    void generateIndices();
    void moveSphere(float deltaTime, float velocity);
    void rotateSphere(float deltaTime);
};

#endif
