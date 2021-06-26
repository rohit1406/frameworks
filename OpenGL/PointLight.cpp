//
//  PointLight.cpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#include "PointLight.hpp"

PointLight::PointLight(): Light(){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(
                       GLuint shadowWidth, GLuint shadowHeight,
                       GLfloat near, GLfloat far,
                       GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat exp): Light(shadowWidth, shadowHeight, red, green, blue, newAmbientIntensity, newDiffuseIntensity){
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
    
    farPlane = far;
    
    float aspect = (float) shadowWidth / (float) shadowHeight;
    lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    
    shadowMap = new OmniShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
}

void PointLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                          GLuint diffuseIntensityLocation, GLuint positionLocation,
                          GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation){
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight(){
    
}

std::vector<glm::mat4> PointLight::calculateLightTransform(){
    std::vector<glm::mat4> lightMatrices;
    // +x, -x
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    
    // +y, -y
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    
    // +z, -z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    
    return lightMatrices;
}

GLfloat PointLight::getFarPlane(){
    return farPlane;
}

glm::vec3 PointLight::getPosition(){
    return position;
}
