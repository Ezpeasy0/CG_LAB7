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

// Camera globals
glm::vec3 cameraPos(1.0f, 0.5f, 2.0f);
glm::vec3 cameraTarget(0.0f, -0.3f, -1.0f);
glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

float yaw = -90.0f;
float pitch = -15.0f;
double lastX = WIDTH / 2.0;
double lastY = HEIGHT / 2.0;
bool firstMouse = true;

// Mouse drag globals
bool isDragging = false;
float sensitivity = 0.1f;

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
    obj1->CreateMesh(vertices, indices, 12, 12); // 4 vertices * 3, 4 triangles * 3

    // Push the same object 10 times
    for (int i = 0; i < 10; i++)
    {
        meshList.push_back(obj1);
    }
}

// Create shader program
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS) isDragging = true;
        else if (action == GLFW_RELEASE) isDragging = false;
    }
}

int main()
{
    // Initialize window
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create geometry and shaders
    CreateTriangle();
    CreateShaders();

    // Shader uniform locations
    shaderList[0].UseShader();
    GLuint uniformModel = shaderList[0].GetUniformLocation("model");
    GLuint uniformProjection = shaderList[0].GetUniformLocation("projection");
    GLuint uniformView = shaderList[0].GetUniformLocation("view");

    // Projection matrix (perspective)
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f, 100.0f
    );

    // Pyramid positions
    glm::vec3 pyramidPositions[] =
    {
        glm::vec3(0.0f, 0.0f, -2.5f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // Set mouse button callback
    glfwSetMouseButtonCallback(mainWindow.getWindow(), mouse_button_callback);

    // Main render loop
    while (!mainWindow.getShouldClose())
    {
        // Poll events
        glfwPollEvents();

        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera direction if dragging
        double mouseX, mouseY;
        glfwGetCursorPos(mainWindow.getWindow(), &mouseX, &mouseY);
        if (isDragging)
        {
            if (firstMouse)
            {
                lastX = mouseX;
                lastY = mouseY;
                firstMouse = false;
            }

            float xoffset = mouseX - lastX;
            float yoffset = lastY - mouseY; // Y reversed
            lastX = mouseX;
            lastY = mouseY;

            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraDirection.y = sin(glm::radians(pitch));
            cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraDirection = glm::normalize(cameraDirection);

            cameraRight = glm::normalize(glm::cross(cameraDirection, up));
            cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
        }
        else
        {
            firstMouse = true; // reset when not dragging
        }

        // Calculate view matrix manually
        glm::mat4 cameraPosMat(1.0f);
        cameraPosMat[3][0] = -cameraPos.x;
        cameraPosMat[3][1] = -cameraPos.y;
        cameraPosMat[3][2] = -cameraPos.z;

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);

        glm::mat4 view = cameraRotateMat * cameraPosMat;

        // Use shader
        shaderList[0].UseShader();

        // Draw 10 pyramids
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pyramidPositions[i]);
            model = glm::rotate(model, glm::radians(2.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
