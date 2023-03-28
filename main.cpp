#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Shader.h"
#include "include/Texture2D.h"

#include <filesystem>
#include <unistd.h>


void framebuffer_size_callback(GLFWwindow *window,int width,int height);
void update(GLFWwindow *window);
void key_callback(GLFWwindow *window,int key, int scancode, int action, int mods);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800,600,"hello window", nullptr, nullptr);
    if (window == nullptr){
        std::cout<<"failed to create window"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    glfwSetKeyCallback(window,key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout<<"failed to initialize GLAD"<<std::endl;
        glfwTerminate();
        return -1;
    }

    float vertices[] = {
            0.5f,  0.5f, 0.0f,  1,1,// top right
            0.5f, -0.5f, 0.0f,  1,0,// bottom right
            -0.5f, -0.5f, 0.0f,  0,0,// bottom left
            -0.5f,  0.5f, 0.0f,   0,1// top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

    Shader shader("/home/matf-racunarska-grafika/Desktop/rg_projekat/resources/shaders/vertexShader.vs",
                  "/home/matf-racunarska-grafika/Desktop/rg_projekat/resources/shaders/fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Texture2D texture2D("/home/matf-racunarska-grafika/Desktop/rg_projekat/resources/textures/container.jpg");

    shader.use();
    shader.setUniform1i("t0",0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2,0.1,0.3,1);
        glClear(GL_COLOR_BUFFER_BIT);

//        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    shader.deleteProgram();
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window,int width,int height){
    glViewport(0,0,width,height);
}

void update(GLFWwindow *window){

}

void key_callback(GLFWwindow *window,int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        std::cerr<<"W";
    }
}