//
//  Mesh.hpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>

#include <GL/glew.h>

class Mesh{
public:
    Mesh();
    
    void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void renderMesh();
    void clearMesh();
    
    ~Mesh();
    
    GLuint VAO;
private:
    GLuint VBO, IBO;
    GLsizei indexCount;
};
#endif /* Mesh_hpp */
