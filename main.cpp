#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Shader.h"
#include "include/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

    //generating texture
    unsigned int tex0;
    glGenTextures(1,&tex0);
    glBindTexture(GL_TEXTURE_2D,tex0);

    //wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

    //filter
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //load img
    int width,height,nChannel;
    unsigned char* data = stbi_load("/home/matf-racunarska-grafika/Desktop/rg_projekat/resources/textures/container.jpg",&width,&height,&nChannel,0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr<<"failed to load texture";
        return -1;
    }

    stbi_image_free(data);

    // scale -> translate
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f),glm::vec3(1,0,0));
    view = glm::translate(view, glm::vec3(0,0,-3));
    projection = glm::perspective(glm::radians(45.0f),(float)800/600,0.1f,100.f);

    shader.use();
//    shader.setUniform1i("t0",0);
    unsigned int modelLoc = glGetUniformLocation(shader.getId(),"model");
    unsigned int viewLoc = glGetUniformLocation(shader.getId(),"view");


    //    glUniformMatrix4fv(locationId,1, GL_FALSE,glm::value_ptr(m));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    shader.setMat4("projection",projection);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2,0.1,0.3,1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0);

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