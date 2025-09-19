#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex Shader & Fragment Shader paths
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

// Create Indexed Triangle (Pyramid-like)
void CreateTriangle()
{
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,  // 0
         0.0f, -1.0f, 1.0f,  // 1
         1.0f, -1.0f, 0.0f,  // 2
         0.0f,  1.0f, 0.0f   // 3 top
    };

    unsigned int indices[] =
    {
        0, 3, 1,   // triangle 0
        1, 3, 2,   // triangle 1
        2, 3, 0,   // triangle 2
        0, 1, 2    // base
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12); // 4 vertices * 3 = 12 floats, 4 triangles * 3 = 12 indices
    meshList.push_back(obj1);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    // Initialize window
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    // Create geometry and shaders
    CreateTriangle();
    CreateShaders();

    // Get uniform locations
    shaderList[0].UseShader();
    GLuint uniformModel = shaderList[0].GetUniformLocation("model");
    GLuint uniformProjection = shaderList[0].GetUniformLocation("projection");

    // Projection matrix
    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f, 100.0f
    );

    // Render loop
    while (!mainWindow.getShouldClose())
    {
        // Poll events
        glfwPollEvents();

        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        shaderList[0].UseShader();

        // Model matrix
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // ขยาย 2 เท่า
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

        // Send matrices to shader
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw mesh
        meshList[0]->RenderMesh();

        glUseProgram(0);

        // Swap buffers
        mainWindow.swapBuffers();
    }

    return 0;
}
