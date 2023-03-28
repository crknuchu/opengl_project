#ifndef RG_PROJEKAT_TEXTURE2D_H
#define RG_PROJEKAT_TEXTURE2D_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

class Texture2D {
    unsigned int tex0;
public:

    Texture2D(const char* path){
        //generating texture
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
        unsigned char* data = stbi_load(path,&width,&height,&nChannel,0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr<<"failed to load texture";
        }

        stbi_image_free(data);
    }
    void use(){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0);
    }
};

#endif //RG_PROJEKAT_TEXTURE2D_H
