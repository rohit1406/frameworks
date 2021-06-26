//
//  Mesh.cpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#include "Mesh.hpp"

Mesh::Mesh(){
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

Mesh::~Mesh(){
    clearMesh();
}

void Mesh::createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices){
    indexCount = numOfIndices;
    
    // Record Vertex Array Object to use it while drawing
    // Generate Vertex Array Object and Activate it by binding
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Create Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
    
    
    // Generate Vertex Buffer Object and Activate it by binding
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Pass data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);
    
    // Map vertex data and enable the attribute to which data is mapped
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
    glEnableVertexAttribArray(0);
    
    // Texture data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void *) (sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(1);
    
    // Normals data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void *) (sizeof(vertices[0]) * 5));
    glEnableVertexAttribArray(2);
    
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //The Above bind IBO and VBO are now connected to this VAO
    
    // Unbind VAO
    glBindVertexArray(0);
}

void Mesh::renderMesh(){
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Indexed Draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::clearMesh(){
    if(IBO != 0){
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    
    if(VBO != 0){
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    
    if(VAO != 0){
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    
    indexCount = 0;
}
