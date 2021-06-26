//
//  Light.cpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#include "Light.hpp"

Light::Light(){
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight,GLfloat red, GLfloat green, GLfloat blue, GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity){
    shadowMap = new ShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
    
    color = glm::vec3(red, green, blue);
    ambientIntensity = newAmbientIntensity;
    diffuseIntensity = newDiffuseIntensity;
}

Light::~Light(){
    
}
