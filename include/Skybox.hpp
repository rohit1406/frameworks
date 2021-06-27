//
//  Skybox.hpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "logger.h"

class Skybox{
public:
    Skybox();
    Skybox(std::vector<std::string> faceLocations);
    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    ~Skybox();
    
private:
    Mesh* skyMesh;
    Shader* skyShader;
    
    GLuint textureId;
    GLuint uniformProjection, uniformView;
};
#endif /* Skybox_hpp */
