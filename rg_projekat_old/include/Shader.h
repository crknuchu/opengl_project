#ifndef RG_PROJEKAT_SHADER_H
#define RG_PROJEKAT_SHADER_H

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string readFileContents(std::string path){
    std::ifstream in(path);
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

class Shader {
    unsigned m_Id;
public:
    Shader(std::string vertexShaderPath,std::string fragmentShaderPath){

        std::string vsString = readFileContents(vertexShaderPath);
        const char* vertexShaderSource = vsString.c_str();

        unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(vertexShader,512, nullptr,infoLog);
            std::cout <<"VERTEX SHADER COMPILE ERROR "<<infoLog<<std::endl;
        }

        std::string fsString = readFileContents(fragmentShaderPath);
        const char* fragmentShaderSource = fsString.c_str();

        unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(fragmentShader,512, nullptr,infoLog);
            std::cout <<"FRAGMENT SHADER COMPILE ERROR "<<infoLog<<std::endl;
        }

        //linking
        unsigned shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram,vertexShader);
        glAttachShader(shaderProgram,fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(shaderProgram,512, nullptr,infoLog);
            std::cout<<"SHADER PROGRAM LINKING ERROR "<<infoLog<<std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        m_Id = shaderProgram;
    }

    void use(){
        glUseProgram(m_Id);
    }

    void setUniform4f(std::string name, float x, float y, float z, float t){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform4f(uniformId,x,y,z,t);
    }
    void setUniform1i(std::string name,int val){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform1i(uniformId,val);
    }
    void setFloat(std::string name,float val){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform1f(uniformId,val);
    }

    void setInt(std::string name,int val){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform1i(uniformId,val);
    }

    void setMat4(std::string name,glm::mat4 mat){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniformMatrix4fv(uniformId,1,GL_FALSE,&mat[0][0]);
    }

    void setVec3(std::string name,glm::vec3 val){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform3fv(uniformId,1,&val[0]);
    }
    void setVec3(std::string name,float x,float y,float z){
        int uniformId = glGetUniformLocation(m_Id,name.c_str());
        glUniform3f(uniformId,x,y,z);
    }



    void deleteProgram(){
        glDeleteProgram(m_Id);
    }
    unsigned getId(){
        return m_Id;
    }
};

#endif //RG_PROJEKAT_SHADER_H
