#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "file_system.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// CAMERA
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 3.0f;

float yaw = -90.0f;
float pitch = 0.0f;

float speed = 2.5f;
float sensitivity = 50.0f;

// deltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Camera FPS", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("src/praktikum5/6.3.coordinate_systems.vs",
                     "src/praktikum5/6.3.coordinate_systems.fs");

    float vertices[] = {
        -0.5f,-0.5f,-0.5f,0,0, 0.5f,-0.5f,-0.5f,1,0, 0.5f,0.5f,-0.5f,1,1,
        0.5f,0.5f,-0.5f,1,1, -0.5f,0.5f,-0.5f,0,1, -0.5f,-0.5f,-0.5f,0,0,
        -0.5f,-0.5f,0.5f,0,0, 0.5f,-0.5f,0.5f,1,0, 0.5f,0.5f,0.5f,1,1,
        0.5f,0.5f,0.5f,1,1, -0.5f,0.5f,0.5f,0,1, -0.5f,-0.5f,0.5f,0,0,
        -0.5f,0.5f,0.5f,1,0, -0.5f,0.5f,-0.5f,1,1, -0.5f,-0.5f,-0.5f,0,1,
        -0.5f,-0.5f,-0.5f,0,1, -0.5f,-0.5f,0.5f,0,0, -0.5f,0.5f,0.5f,1,0,
        0.5f,0.5f,0.5f,1,0, 0.5f,0.5f,-0.5f,1,1, 0.5f,-0.5f,-0.5f,0,1,
        0.5f,-0.5f,-0.5f,0,1, 0.5f,-0.5f,0.5f,0,0, 0.5f,0.5f,0.5f,1,0,
        -0.5f,-0.5f,-0.5f,0,1, 0.5f,-0.5f,-0.5f,1,1, 0.5f,-0.5f,0.5f,1,0,
        0.5f,-0.5f,0.5f,1,0, -0.5f,-0.5f,0.5f,0,0, -0.5f,-0.5f,-0.5f,0,1,
        -0.5f,0.5f,-0.5f,0,1, 0.5f,0.5f,-0.5f,1,1, 0.5f,0.5f,0.5f,1,0,
        0.5f,0.5f,0.5f,1,0, -0.5f,0.5f,0.5f,0,0, -0.5f,0.5f,-0.5f,0,1
    };

    glm::vec3 cubePositions[] = {
        {0,0,0},{2,5,-15},{-1.5,-2.2,-2.5},{-3.8,-2,-12.3},{2.4,-0.4,-3.5},
        {-1.7,3,-7.5},{1.3,-2,-2.5},{1.5,2,-2.5},{1.5,0.2,-1.5},{-1.3,1,-1.5}
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f,0.3f,0.3f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                (float)SCR_WIDTH/SCR_HEIGHT,0.1f,100.0f);

        glm::vec3 cameraPos(cameraX,cameraY,cameraZ);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        ourShader.setMat4("projection",projection);
        ourShader.setMat4("view",view);

        glBindVertexArray(VAO);

        for(int i=0;i<10;i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model,cubePositions[i]);
            model = glm::rotate(model,glm::radians(20.0f*i),
                    glm::vec3(1.0f,0.3f,0.5f));

            ourShader.setMat4("model",model);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

// INPUT
void processInput(GLFWwindow *window)
{
    float velocity = speed * deltaTime;
    glm::vec3 cameraPos(cameraX,cameraY,cameraZ);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // ROTASI
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) yaw -= sensitivity * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) yaw += sensitivity * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) pitch += sensitivity * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) pitch -= sensitivity * deltaTime;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    // GERAK
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += velocity * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= velocity * cameraFront;

    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= right * velocity;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos.y += velocity;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos.y -= velocity;

    cameraX = cameraPos.x;
    cameraY = cameraPos.y;
    cameraZ = cameraPos.z;
}

// RESIZE
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}