//
//  Texture.cpp
//  OpenGL
//
//  Created by Apple on 02/06/21.
//

#include "Texture.hpp"

Texture::Texture(){
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

Texture::Texture(const char* fileLoc){
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = fileLoc;
}

bool Texture::loadTexture(){
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if(!texData){
        printf("Failed to find %s!", fileLocation);
        return false;
    }
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // zoom out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // zoom in
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Loads Image data into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    
    glGenerateMipmap(GL_TEXTURE_2D);    // Automatically generates mipmaps
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Image data is already pass to texture, so it is better to clean that memory
    stbi_image_free(texData);
    return true;
}

bool Texture::loadTextureA(){
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if(!texData){
        printf("Failed to find %s!", fileLocation);
        return false;
    }
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // zoom out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // zoom in
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Loads Image data into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    
    glGenerateMipmap(GL_TEXTURE_2D);    // Automatically generates mipmaps
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Image data is already pass to texture, so it is better to clean that memory
    stbi_image_free(texData);
    return true;
}

void Texture::clearTexture(){
    glDeleteTextures(1, &textureId);
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

void Texture::useTexture(){
    // Binding textureId to texture unit 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
Texture::~Texture(){
    clearTexture();
}
