//
//  ShadowMap.cpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#include "ShadowMap.hpp"

ShadowMap::ShadowMap(){
    FBO = 0;
    shadowMap = 0;
}

bool ShadowMap::init(GLuint width, GLuint height){
    shadowWidth = width;
    shadowHeight = height;
    
    glGenFramebuffers(1, &FBO);
    
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    
    // Every time a FBO is bound, we output the image to below texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // zoom out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   // zoom in
    float bColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // connects the framebuffer to our texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    
    // Our framebuffer don't need to draw and read from color attachments
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        LOGGER("Framebuffer Error: "+ std::to_string(status));
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LOGGER("ShadowMap Initialised.");
    return true;
}

void ShadowMap::write(){
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::read(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
}

ShadowMap::~ShadowMap(){
    if(FBO){
        glDeleteFramebuffers(1, &FBO);
    }
    
    if(shadowMap){
        glDeleteTextures(1, &shadowMap);
    }
}
