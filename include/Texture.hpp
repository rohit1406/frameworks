//
//  Texture.hpp
//  OpenGL
//
//  Created by Apple on 02/06/21.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glew.h>
#include "CommonValues.h"

class Texture{
public:
    Texture();
    Texture(const char* fileLoc);
    
    bool loadTexture();
    bool loadTextureA();
    
    void useTexture();
    void clearTexture();
    
    ~Texture();
    
private:
    GLuint textureId;
    int width, height, bitDepth;
    
    const char* fileLocation;
};
#endif /* Texture_hpp */
