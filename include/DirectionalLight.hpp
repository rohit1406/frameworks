//
//  DirectionalLight.hpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include <stdio.h>

#include "Light.hpp"

class DirectionalLight : public Light{
public:
    DirectionalLight();
    DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);
    void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
                  GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    
    glm::mat4 calculateLightTransform();
    ~DirectionalLight();
    
private:
    glm::vec3 direction;
};

#endif /* DirectionalLight_hpp */
