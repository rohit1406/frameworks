//
//  SpotLight.hpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <stdio.h>
#include "PointLight.hpp"

class SpotLight: public PointLight{
public:
    SpotLight();
    SpotLight(
              GLuint shadowWidth, GLuint shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
               GLfloat newAmbientIntensity, GLfloat newDiffuseIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
               GLfloat con, GLfloat lin, GLfloat exp, GLfloat newEdge);
    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint directionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                  GLuint edgeLocation);
    
    void setFlash(glm::vec3 pos, glm::vec3 dir);
    void toggle(){
        isOn = !isOn;
    }
    ~SpotLight();
    
private:
    glm::vec3 direction;
    
    GLfloat edge, procEdge;
    
    bool isOn;
};
#endif /* SpotLight_hpp */
