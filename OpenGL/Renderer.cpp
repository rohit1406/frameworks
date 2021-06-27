//
//  Renderer.cpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#include "Renderer.hpp"

Renderer::Renderer(){
    
}

Renderer::Renderer(CustomWindow *window){
    this->window = window;
}

void Renderer::init(){
    createObjects();
    createShaders();
    
    brickTexture = Texture("Textures/brick_red.jpg");
    brickTexture.loadTexture();
    dirtTexture = Texture("Textures/ground_dirt.jpg");
    dirtTexture.loadTexture();
    planeTexture = Texture("Textures/plain.png");
    planeTexture.loadTextureA();
    LOGGER("Textures Loaded");
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 1.0f, 1.0f,          // Light Color
                                 0.1f,                      // Ambient Intensity
                                 0.9f,                      // Diffuse Intensity
                                 0.0f, -100.0f, 0.0f         // Light Direction
                        );
    
    
    pointLights[0] = PointLight(
                                1024, 1024,
                                0.01f, 100.0f,
                                0.0f, 0.0f, 1.0f,           // Color of Light
                                0.2f,                       // Ambient Intensity
                                1.0f,                       // Diffuse Intensity
                                1.0f, 2.0f, 0.0f,          // Light Position
                                0.3f,                       // constant
                                0.2f,                       // linear
                                0.1f                        // Exponent
                                );
    //pointLightCount++;
    
    pointLights[1] = PointLight(
                                1024, 1024,
                                0.01f, 100.0f,
                                0.0f, 1.0f, 0.0f,           // Color of Light
                                0.2f,                       // Ambient Intensity
                                1.0f,                       // Diffuse Intensity
                                -4.0f, 3.0f, 0.0f,          // Light Position
                                0.3f,                       // constant
                                0.2f,                       // linear
                                0.1f                        // Exponent
                                );
    //pointLightCount++;
    
    spotLights[0] = SpotLight(
                              1024, 1024,
                              0.01f, 100.0f,
                             1.0f, 1.0f, 1.0f,
                             0.0f, 6.0f,
                             0.0f, 0.0f, 0.0f,
                             0.0f, -1.0f, 0.0f,             // Direction of spot light
                             1.0f, 0.0f, 0.0f,
                             20.0f                          // Edge
                             );
    //spotLightCount++;
    
    spotLights[1] = SpotLight(
                              1024, 1024,
                              0.01f, 100.0f,
                             1.0f, 1.0f, 1.0f,
                             0.0f, 6.0f,
                             0.0f, -1.5f, 0.0f,
                             -100.0f, -1.0f, 0.0f,             // Direction of spot light
                             1.0f, 0.0f, 0.0f,
                             20.0f                          // Edge
                             );
    //spotLightCount++;
    LOGGER("Lighting added.");
    
    // Load Skybox
    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("Textures/Skybox/sh_rt.png");
    skyboxFaces.push_back("Textures/Skybox/sh_lf.png");
    skyboxFaces.push_back("Textures/Skybox/sh_up.png");
    skyboxFaces.push_back("Textures/Skybox/sh_dn.png");
    skyboxFaces.push_back("Textures/Skybox/sh_bk.png");
    skyboxFaces.push_back("Textures/Skybox/sh_ft.png");
    skybox = Skybox(skyboxFaces);
    
    shinyMaterial = Material(4.0f, 156);
    dullMaterial = Material(0.3f, 4);
    
    modelE45AirCraft = Model();
    modelE45AirCraft.loadModel("Models/E-45-Aircraft_obj.obj");
    modelAVMT300 = Model();
    modelAVMT300.loadModel("Models/AVMT300.obj");
    LOGGER("Models Created.");
    // Create Projection Matrix
    projection = glm::perspective(glm::radians(60.0f), (GLfloat) window->getBufferWidth() / (GLfloat) window->getBufferHeight(), 0.1f, 100.0f);
    
    // Font Renderer
    fontRenderer = new FontRenderer();
    fontRenderer->init();
}

void Renderer::render(Camera camera){
    if(window->getKeys()[GLFW_KEY_L]){
        spotLights[0].toggle();
        window->getKeys()[GLFW_KEY_L] = false;
    }
    
    // Render to framebuffer
    directionalShadowMapPass(&mainLight);
    for(size_t i=0; i<pointLightCount; i++){
        omniDirectionalShadowMapPass(&pointLights[i]);
    }
    for(size_t i=0; i<spotLightCount; i++){
        omniDirectionalShadowMapPass(&spotLights[i]);
    }
    
    // Render to screen
    renderPass(camera, projection);
    
    // UnBind shader program
    glUseProgram(0);
}

void Renderer::calculateAverageNormals(unsigned int *indices, unsigned int indicesCount, GLfloat *vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset){
    for(size_t i=0; i<indicesCount; i+=3){
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i+1] * vLength;
        unsigned int in2 = indices[i+2] * vLength;
        
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1],
                     vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1],
                     vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);
        
        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        
        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;
        
        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;
        
        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }
    
    for(size_t i=0; i<verticesCount/vLength; i++){
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset+1], vertices[nOffset+2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset+1] = vec.y;
        vertices[nOffset+2] = vec.z;
    }
}

void Renderer::createObjects(){
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    
    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };
    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };
    calculateAverageNormals(indices, 12, vertices, 32, 8, 5);
    
    Mesh* obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh* obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);
    
    Mesh* obj3 = new Mesh();
    obj3->createMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
    
    LOGGER("Objection creation successful.");
}

void Renderer::createShaders(){
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
    directionalShadowShader = Shader();
    directionalShadowShader.createFromFiles("Shaders/DirectionalShadowMap.vert", "Shaders/DirectionalShadowMap.frag");
    
    // Create Omni Directional Shadow Shaders
    omniShadowShader = Shader();
    omniShadowShader.createFromFiles("Shaders/OmniShadowMap.vert", "Shaders/OmniShadowMap.geom", "Shaders/OmniShadowMap.frag");
    LOGGER("Shader creation successful.");
}

void Renderer::renderScene(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->renderMesh();
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -3.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->renderMesh();
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.useTexture();
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->renderMesh();

    e45Angle += 0.1f;
    if(e45Angle > 360){
        e45Angle -= 360;
    }
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(e45Angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-8.0f, 1.0f, -2.0f));
    model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    modelAVMT300.renderModel();
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    modelE45AirCraft.renderModel();
}

void Renderer::directionalShadowMapPass(DirectionalLight *light){
    directionalShadowShader.useShader();
    
    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());
    
    light->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformModel = directionalShadowShader.getModelLocation();
    glm::mat4 lTransform = light->calculateLightTransform();
    directionalShadowShader.setDirectionalLightTransform(&lTransform);
    directionalShadowShader.validate(meshList[0]->VAO);
    renderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::omniDirectionalShadowMapPass(PointLight *light){
    omniShadowShader.useShader();
    
    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());
    
    light->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformModel = omniShadowShader.getModelLocation();
    uniformOmniLightPos = omniShadowShader.getOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.getFarPlaneLocation();
    
    glUniform3f(uniformOmniLightPos, light->getPosition().x, light->getPosition().y, light->getPosition().z);
    glUniform1f(uniformFarPlane, light->getFarPlane());
    omniShadowShader.setLightMatrices(light->calculateLightTransform());
    omniShadowShader.validate(meshList[0]->VAO);
    renderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderPass(Camera camera,glm::mat4 projectionMatrix){
    
    glViewport(0, 0, 800, 600);
    
    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 viewMatrix = camera.calculateViewMatrix();
    // Render Skybox
    skybox.draw(viewMatrix, projectionMatrix);

    // Draw Rest of the things
    shaderList[0].useShader();
    
    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    uniformEyePosition = shaderList[0].getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    uniformShininess = shaderList[0].getShininessLocation();
    
    
    
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
    
    shaderList[0].setDirectionalLight(&mainLight);
    
    // Note: the below sequence of setting first point lights and then spot lights is crutial in order to work the programm correctly
    // because of the processing sequence handled in the shaders
    shaderList[0].setPointLight(pointLights, pointLightCount, 3, 0);
    shaderList[0].setSpotLight(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    
    glm::mat4 lTransform = mainLight.calculateLightTransform();
    shaderList[0].setDirectionalLightTransform(&lTransform);
    
    mainLight.getShadowMap()->read(GL_TEXTURE2);
    shaderList[0].setTexture(1);
    shaderList[0].setDirectionalShadowMap(2);
    
    glm::vec3 lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.3f;
    spotLights[0].setFlash(lowerLight, camera.getCameraDirection());
    
    shaderList[0].validate(meshList[0]->VAO);
    
    renderScene();
    
    fontRenderer->render();
}

Renderer::~Renderer(){
    
}
