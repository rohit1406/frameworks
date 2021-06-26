//
//  Material.hpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>

#include <GL/glew.h>

class Material{
public:
    Material();
    Material(GLfloat intensity, GLfloat shine);
    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
    ~Material();
    
private:
    GLfloat specularIntensity;
    GLfloat shininess;
};
#endif /* Material_hpp */
