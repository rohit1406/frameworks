//
//  PointLight.hpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <stdio.h>

#include "Light.hpp"
#include "OmniShadowMap.hpp"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class PointLight: public Light{
public:
    PointLight();
    PointLight(
               GLuint shadowWidth, GLuint shadowHeight,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue,
               GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);
    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
    std::vector<glm::mat4> calculateLightTransform();
    GLfloat getFarPlane();
    glm::vec3 getPosition();
    ~PointLight();
    
protected:
    glm::vec3 position;
    
    // attenuation calculation
    // ax2 + bx + c ; x is the distance from light, a=exponent, b=linear, c=constant
    GLfloat constant, linear, exponent;
    
    GLfloat farPlane;
};

#endif /* PointLight_hpp */
