#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
//    gladLoadGL(); // window crashes without this
//    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    glfwSetKeyCallback(window,key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout<<"failed to initialize GLAD"<<std::endl;
        glfwTerminate();
        return -1;
    }

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        update(window);
        glfwSwapBuffers(window);
    }

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