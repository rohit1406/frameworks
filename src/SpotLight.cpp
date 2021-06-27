//
//  SpotLight.cpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#include "SpotLight.hpp"

SpotLight::SpotLight(): PointLight(){
    direction= glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    procEdge = cosf(glm::radians(edge));
    isOn = true;
}

SpotLight::SpotLight(
                     GLuint shadowWidth, GLuint shadowHeight,
                     GLfloat near, GLfloat far,
                     GLfloat red, GLfloat green, GLfloat blue,
           GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity,
           GLfloat xPos, GLfloat yPos, GLfloat zPos,
          GLfloat xDir, GLfloat yDir, GLfloat zDir,
          GLfloat con, GLfloat lin, GLfloat exp, GLfloat newEdge): PointLight(shadowWidth, shadowHeight, near, far ,red, green, blue, newAmbientIntensity, newDiffuseIntensity, xPos, yPos, zPos, con, lin, exp){
    direction= glm::normalize(glm::vec3(xDir, yDir, zDir));
    edge = newEdge;
    procEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
              GLuint diffuseIntensityLocation, GLuint positionLocation,
              GLuint directionLocation,
              GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
              GLuint edgeLocation){
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    
    if(isOn){
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    }else{
        glUniform1f(ambientIntensityLocation, 0.0f);
        glUniform1f(diffuseIntensityLocation, 0.0f);
    }
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
    
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, procEdge);
}

SpotLight::~SpotLight(){
    
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir){
    position = pos;
    direction = dir;
}
