#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct ProgramState {
    bool ImGuiEnabled = false;
    glm::vec3 clearColor = glm::vec3(0);

    void LoadFromDisk(std::string path);
    void SaveToDisk(std::string path);
};

void ProgramState::LoadFromDisk(std::string path) {
    std::ifstream in(path);
    if(in){
        in >> ImGuiEnabled
            >> clearColor.r
            >> clearColor.g
            >> clearColor.b;
    }
    std::cerr<<"AAAAAAAAAAAAAAAAAAAa";
}

void ProgramState::SaveToDisk(std::string path) {
    std::ofstream out(path);
    out << ImGuiEnabled << '\n'
        << clearColor.r << '\n'
        << clearColor.g << '\n'
        << clearColor.b;
    std::cerr<<"BBBBBBBBBBBBBBBBB";
}

ProgramState* programState;

void DrawImGui(ProgramState* programState);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "rg_projekat", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //camera goes crazy if this is set to GLFW_CURSOR_DISABLED

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    programState = new ProgramState;
    programState->LoadFromDisk("resources/programState.txt");

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("resources/shaders/1.model_loading.vs", "resources/shaders/1.model_loading.fs");

    Model ourModel(FileSystem::getPath("resources/objects/backpack/backpack.obj"));

    PointLight pointLight;
    pointLight.position = glm::vec3(4,4,0);
    pointLight.ambient = glm::vec3(0.4,0.4,0.2);
    pointLight.diffuse = glm::vec3(0.5,0.6,1.0);
    pointLight.specular = glm::vec3(1.0);
    pointLight.constant = 1.0;
    pointLight.linear = 0.09;
    pointLight.quadratic = 0.032;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        ourShader.setVec3("pointLight.position",pointLight.position);
        ourShader.setVec3("pointLight.ambient",pointLight.ambient);
        ourShader.setVec3("pointLight.diffuse",pointLight.diffuse);
        ourShader.setVec3("pointLight.specular",pointLight.specular);
        ourShader.setFloat("pointLight.constant",pointLight.constant);
        ourShader.setFloat("pointLight.linear",pointLight.linear);
        ourShader.setFloat("pointLight.quadratic",pointLight.quadratic);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        if (programState->ImGuiEnabled)
            DrawImGui(programState);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    programState->SaveToDisk("resources/programState.txt");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    delete programState;
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    if(programState->ImGuiEnabled == false)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void DrawImGui(ProgramState* programState){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0;
        ImGui::Begin("Test Window");
        ImGui::Text("Hello World");
        ImGui::DragFloat("demo slider",&f,0.05,0.0,1.0);
        ImGui::ColorEdit3("background color",(float*)&programState->clearColor);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS){
        programState->ImGuiEnabled = !(programState->ImGuiEnabled);
//        if (programState->ImGuiEnabled) {
//            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
//            return;
//        }
//        else {
//            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
//            return;
//        }
    }
}