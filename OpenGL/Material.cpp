//
//  Material.cpp
//  OpenGL
//
//  Created by Apple on 03/06/21.
//

#include "Material.hpp"

Material::Material(){
    specularIntensity = 0.0f;
    shininess = 0.0f;
}

Material::Material(GLfloat intensity, GLfloat shine){
    specularIntensity = intensity;
    shininess = shine;
}

Material::~Material(){
    
}

void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation){
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}
