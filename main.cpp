#include "loadTexture.h"
#include "physics.h"
#include "shaders.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLFWwindow *window;
static float lastTime = 0.0f;

float sphere1Rotation = 0.0f;
float sphere2Rotation = 0.0f;

float getDeltaTime() {
  float currentTime = glfwGetTime();
  float deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  return deltaTime;
}

// Function to check and handle elastic collision
void checkCollision(glm::vec3 &pos1, glm::vec3 &pos2, float &v1, float &v2,
                    float r1, float r2) {
  float distance = glm::distance(pos1, pos2);
  float combinedRadius = r1 + r2;

  if (distance <= combinedRadius) {
    // Assume mass is proportional to radius
    float m1 = r1;
    float m2 = r2;

    // Elastic collision velocity updates
    float newV1 = v1 * (m1 - m2) / (m1 + m2) + v2 * (2 * m2) / (m1 + m2);
    float newV2 = v2 * (m2 - m1) / (m1 + m2) + v1 * (2 * m1) / (m1 + m2);

    v1 = newV1;
    v2 = newV2;
  }
}

void resetSimulation(bool &isRunning, bool &parametersSet,
                     glm::vec3 &sphere1Pos, glm::vec3 &sphere2Pos,
                     float &sphere1MovementSpeed, float &sphere2MovementSpeed,
                     float &sphere1Radius, float &sphere2Radius,
                     Sphere &sphere1, Sphere &sphere2) {
  isRunning = false;
  parametersSet = false;
  sphere1Pos = glm::vec3(-20.0f, sphere1Radius, 0.0f);
  sphere2Pos = glm::vec3(-10.0f, sphere2Radius, 0.0f);
  sphere1MovementSpeed = 0.3f;
  sphere2MovementSpeed = 0.3f;
  sphere1Radius = 1.0f;
  sphere2Radius = 1.0f;

  // Reset rotation angles
  sphere1Rotation = 0.0f;
  sphere2Rotation = 0.0f;

  sphere1 = Sphere(sphere1Radius, 36, 18);
  sphere2 = Sphere(sphere2Radius, 36, 18);
}

// Add ground plane vertex data
float groundVertices[] = {
    // Positions          // Texture Coords
    -50.0f, -3.0f, -50.0f, 0.0f, 0.0f, // Bottom-left
    50.0f,  -3.0f, -50.0f, 1.0f, 0.0f, // Bottom-right
    50.0f,  -3.0f, 50.0f,  1.0f, 1.0f, // Top-right
    -50.0f, -3.0f, 50.0f,  0.0f, 1.0f  // Top-left
};

unsigned int groundIndices[] = {
    0, 1, 2, // First triangle
    2, 3, 0  // Second triangle
};

// Function to create and configure the ground plane VAO and VBO
void setupGroundPlane(GLuint &groundVAO, GLuint &groundVBO, GLuint &groundEBO) {
  glGenVertexArrays(1, &groundVAO);
  glGenBuffers(1, &groundVBO);
  glGenBuffers(1, &groundEBO);

  glBindVertexArray(groundVAO);

  glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices,
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

// Function to render the ground plane
void renderGroundPlane(GLuint groundVAO, GLuint groundTexture, GLuint programID,
                       GLuint MatrixID, glm::mat4 MVP) {
  glUseProgram(programID);
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // Bind the texture
  glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
  glBindTexture(GL_TEXTURE_2D, groundTexture);

  // Set the texture sampler in the shader to use texture unit 0
  GLuint textureLocation = glGetUniformLocation(programID, "textureSampler");
  glUniform1i(textureLocation, 0);

  // Render the ground plane
  glBindVertexArray(groundVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

int main() {
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

  window = glfwCreateWindow(1920, 1080, "Sphere Simulation", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    return -1;
  }

  // Setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  glEnable(GL_DEPTH_TEST);

  GLuint programID =
      LoadShaders("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
  glm::mat4 View =
      glm::lookAt(glm::vec3(0, 0, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  float sphere1Radius = 1.0f;
  float sphere2Radius = 1.0f;
  float sphere1MovementSpeed = 0.3f;
  float sphere2MovementSpeed = 0.3f;
  bool isRunning = false;
  bool parametersSet = false;

  glm::vec3 sphere1Pos(-20.0f, sphere1Radius - 3.0f, 0.0f);
  glm::vec3 sphere2Pos(-10.0f, sphere2Radius - 3.0f, 0.0f);

  Sphere sphere1(sphere1Radius, 36, 18);
  Sphere sphere2(sphere2Radius, 36, 18);

  GLuint texture1 = loadBMP_custom("textures/ball1.bmp");
  GLuint texture2 = loadBMP_custom("textures/ball2.bmp");
  GLuint groundTexture = loadBMP_custom("textures/concrete.bmp");
  sphere1.setTexture(texture1);
  sphere2.setTexture(texture2);

  GLuint groundVAO, groundVBO, groundEBO;
  setupGroundPlane(groundVAO, groundVBO, groundEBO);
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float deltaTime = getDeltaTime();

    if (isRunning) {
      // Update sphere positions
      sphere1Pos.x += sphere1MovementSpeed * deltaTime;
      sphere2Pos.x += sphere2MovementSpeed * deltaTime;

      // Check for collision
      checkCollision(sphere1Pos, sphere2Pos, sphere1MovementSpeed,
                     sphere2MovementSpeed, sphere1Radius, sphere2Radius);

      if (sphere1MovementSpeed != 0.0f) {
        sphere1Rotation +=
            (sphere1MovementSpeed / sphere1Radius) * deltaTime * 2.0f;
      }
      if (sphere2MovementSpeed != 0.0f) {
        sphere2Rotation +=
            (sphere2MovementSpeed / sphere2Radius) * deltaTime * 2.0f;
      }
    }

    // Create transformation matrices
    glm::mat4 Model1 = glm::translate(glm::mat4(1.0f), sphere1Pos);
    Model1 = glm::rotate(Model1, -sphere1Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 MVP1 = Projection * View * Model1;

    glm::mat4 Model2 = glm::translate(glm::mat4(1.0f), sphere2Pos);
    Model2 = glm::rotate(Model2, -sphere2Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 MVP2 = Projection * View * Model2;

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
    sphere1.draw(programID, MatrixID, MVP1);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
    sphere2.draw(programID, MatrixID, MVP2);

    // Render ground plane
    glm::mat4 groundModel = glm::mat4(1.0f); // Identity matrix for ground
    glm::mat4 groundMVP = Projection * View * groundModel;
    renderGroundPlane(groundVAO, groundTexture, programID, MatrixID, groundMVP);

    // ImGui UI Rendering
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Sphere 1 Controls");
    ImGui::SliderFloat("Speed", &sphere1MovementSpeed, 0.0f, 2.0f);
    ImGui::SliderFloat("Radius", &sphere1Radius, 0.5f, 4.0f);
    ImGui::End();

    ImGui::Begin("Sphere 2 Controls");
    ImGui::SliderFloat("Speed", &sphere2MovementSpeed, 0.0f, 2.0f);
    ImGui::SliderFloat("Radius", &sphere2Radius, 0.5f, 4.0f);
    ImGui::End();

    ImGui::Begin("Simulation Controls");
    if (!parametersSet) {
      if (ImGui::Button("Set Parameters")) {
        sphere1.~Sphere(); // Explicitly destroy old object
        sphere2.~Sphere();
        new (&sphere1) Sphere(sphere1Radius, 36, 18);
        new (&sphere2) Sphere(sphere2Radius, 36, 18);
        sphere1.setTexture(texture1);
        sphere2.setTexture(texture2);
        parametersSet = true;
        sphere1Pos.y = sphere1Radius - 3.0f; // Keep bottom aligned
        sphere2Pos.y = sphere2Radius - 3.0f; // Keep bottom aligned
      }
    } else {
      if (ImGui::Button(isRunning ? "Pause Simulation" : "Start Simulation")) {
        isRunning = !isRunning;
      }
      if (ImGui::Button("Reset Simulation")) {
        resetSimulation(isRunning, parametersSet, sphere1Pos, sphere2Pos,
                        sphere1MovementSpeed, sphere2MovementSpeed,
                        sphere1Radius, sphere2Radius, sphere1, sphere2);
      }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (!glfwWindowShouldClose(window));

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
