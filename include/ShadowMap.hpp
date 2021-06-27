//
//  ShadowMap.hpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#ifndef ShadowMap_hpp
#define ShadowMap_hpp

#include <stdio.h>
#include <GL/glew.h>

#include "logger.h"

class ShadowMap{
public:
    ShadowMap();
    virtual bool init(GLuint width, GLuint height);
    virtual void write();
    virtual void read(GLenum textureUnit);
    
    GLuint getShadowWidth(){
        return shadowWidth;
    }
    
    GLuint getShadowHeight(){
        return shadowHeight;
    }
    ~ShadowMap();
    
protected:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};
#endif /* ShadowMap_hpp */
