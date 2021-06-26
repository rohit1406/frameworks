//
//  Light.hpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.hpp"

class Light{
public:
    Light();
    Light(GLuint shadowWidth, GLuint shadowHeight,
          GLfloat red, GLfloat green, GLfloat blue, GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity);
    
    ShadowMap* getShadowMap(){
        return shadowMap;
    }
    
    ~Light();
    
protected:
    // Ambient Light doesn't rely on the location of the light source
    // It is like a Universal lighting system around the whole world
    glm::vec3 color;
    GLfloat ambientIntensity;
    
    // Diffuse lighting dependends on where the light actually coming from
    GLfloat diffuseIntensity;
    
    glm::mat4 lightProj;
    ShadowMap *shadowMap;
};
#endif /* Light_hpp */
