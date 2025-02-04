#include "physics.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectors(sectors), stacks(stacks) {

  generateSphere();
}

Sphere::~Sphere() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteBuffers(1, &indexBuffer);
  glDeleteBuffers(1, &textureBuffer);
  glDeleteVertexArrays(1, &vertexArrayID);
}
void Sphere::generateSphere() {
  generateVertices();
  generateColors();
  generateIndices();

  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &textureBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float),
               textureCoords.data(), GL_STATIC_DRAW);
}

void Sphere::generateVertices() {
  float x, y, z, xy;                           // Vertex position (x, y, z)
  float nx, ny, nz, lengthInv = 1.0f / radius; // Normalized vertex
  float sectorStep = 2 * M_PI / sectors;       // Angle step
  float stackStep = M_PI / stacks;             // Angle step

  // Loop through the stacks
  for (int i = 0; i <= stacks; ++i) {
    float stackAngle = M_PI / 2 - i * stackStep; // Current angle
    xy = radius * cosf(stackAngle);              // Projected radius
    z = radius * sinf(stackAngle);               // Z position

    // Loop through the sectors
    for (int j = 0; j <= sectors; ++j) {
      float sectorAngle = j * sectorStep; // Current angle

      // Calculate position
      x = xy * cosf(sectorAngle); // X position
      y = xy * sinf(sectorAngle); // Y position

      // Normalized vertex
      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;

      // Compute texture coordinates (UV mapping)
      float u = (float)j / sectors; // U coordinate
      float v = (float)i / stacks;  // V coordinate

      // Add vertex data
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      // Add normal data (optional, if using lighting)
      // normals.push_back(nx);
      // normals.push_back(ny);
      // normals.push_back(nz);

      // Add color data (optional)
      colors.push_back(0.5f + 0.5f * nx);
      colors.push_back(0.5f + 0.5f * ny);
      colors.push_back(0.5f + 0.5f * nz);

      // Add texture coordinates
      textureCoords.push_back(u);
      textureCoords.push_back(v);
    }
  }
}

void Sphere::generateColors() {
  // Color data generation logic is already handled inside generateVertices()
}

void Sphere::generateIndices() {
  // Loop through the stacks and sectors to create the indices
  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < sectors; ++j) {
      int first = i * (sectors + 1) + j;
      int second = first + sectors + 1;
      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(second);
      indices.push_back(second + 1);
      indices.push_back(first + 1);
    }
  }
}

void Sphere::updatePosition(glm::vec3 position) { this->position = position; }

bool Sphere::checkCollision(const Sphere &otherSphere) {
  // Calculate the distance between the centers of the spheres

  float distance = glm::distance(this->position, otherSphere.position);

  std::cout << this->position.x << " , " << otherSphere.position.x << " , "
            << std::endl;

  // std::cout << distance;
  //  Check if the distance is less than the sum of the radii
  if (distance < this->radius + otherSphere.radius) {
    return true; // Collision detected
  }
  return false; // No collision
}

void Sphere::update(float deltaTime, float velocity) {
  moveSphere(deltaTime, velocity); // Move along x-axis
  rotateSphere(deltaTime);         // Rotate on the y-axis
}

void Sphere::moveSphere(float deltaTime, float velocity) {
  position.x +=
      velocity *
      deltaTime; // Move sphere along x-axis based on velocity and time delta
}

void Sphere::rotateSphere(float deltaTime) {
  angle += 45.0f * deltaTime; // Rotate by 45 degrees per second (you can adjust
                              // the speed here)
  if (angle > 360.0f) {
    angle -= 360.0f;
  }
}

void Sphere::setTexture(GLuint textureID) { this->textureID = textureID; }

void Sphere::draw(GLuint programID, GLuint MatrixID, const glm::mat4 &MVP) {
  glUseProgram(programID);
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glBindVertexArray(vertexArrayID);

  // Position attribute
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  // Color attribute (optional, remove if using textures fully)
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  // Texture coordinates attribute
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

  // Bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(glGetUniformLocation(programID, "texture1"), 0);

  // Draw sphere
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

  // Cleanup
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
