//
//  Renderer.hpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>                 // to pass the values to Shaders

#include "CustomWindow.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Skybox.hpp"
#include "FontRenderer.hpp"
#include "logger.h"

class Renderer{
public:
    Renderer();
    Renderer(CustomWindow *window);
    void init();
    void render(Camera camera);
    ~Renderer();
    
private:
    CustomWindow *window;
    glm::mat4 projection;
    // UNIFORMS
    GLuint uniformProjection =0, uniformModel = 0, uniformView = 0;
    GLuint uniformEyePosition =0, uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;
    
    // SHADERS
    std::vector<Shader> shaderList;     // Shader list for handling the general scene
    Shader directionalShadowShader;     // Shader for handling Directional Shadows
    Shader omniShadowShader;            // Shader for handling Omni Directional Shadows
    
    // MODEL & MESH
    std::vector<Mesh*> meshList;
    // Vertex Shader
    const char* vShader = "Shaders/shader.vert";
    // Fragment Shader
    const char* fShader = "Shaders/shader.frag";
    Model modelE45AirCraft;
    Model modelAVMT300;

    // TEXTURES
    Texture brickTexture;
    Texture dirtTexture;
    Texture planeTexture;

    // MATERIALS
    Material shinyMaterial;
    Material dullMaterial;
    
    // LIGHT
    DirectionalLight mainLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
    SpotLight spotLights[MAX_SPOT_LIGHTS];

    unsigned int pointLightCount = 0;
    unsigned int spotLightCount = 0;

    // SKYBOX
    Skybox skybox;
    
    // Others
    GLfloat e45Angle = 0.0f;
    
    // Text Renderer
    FontRenderer* fontRenderer;
    
    
    // Functions
    void createShaders();
    void renderScene();
    void directionalShadowMapPass(DirectionalLight *light);
    void omniDirectionalShadowMapPass(PointLight *light);
    void createObjects();
    void calculateAverageNormals(unsigned int *indices, unsigned int indicesCount, GLfloat *vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset);
    void renderPass(Camera camera, glm::mat4 projectionMatrix);
    
};
#endif /* Renderer_hpp */
