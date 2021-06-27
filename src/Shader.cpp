//
//  Shader.cpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#include "Shader.hpp"

Shader::Shader(){
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
    
    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode){
    compileShader(vertexCode, fragmentCode);
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode){
    shaderId = glCreateProgram();
    if(!shaderId){
        LOGGER("Failed to Create a Shader Program Object!");
        return;
    }
    
    addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);
    
    compileProgram();
}

void Shader::compileShader(const char *vertexCode, const char* geometryCode, const char *fragmentCode){
    shaderId = glCreateProgram();
    if(!shaderId){
        LOGGER("Failed to Create a Shader Program Object!");
        return;
    }
    
    addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderId, geometryCode, GL_GEOMETRY_SHADER);
    addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);
    
    compileProgram();
}

void Shader::validate(GLuint VAO){
    
    glBindVertexArray(VAO);
    GLint result = 0;
    GLchar eLog[1024] = {0};
    // Validate the program
    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        LOGGER("Failed to validate the program: %s\n"+ std::string(eLog));
        return;
    }
    
}

void Shader::compileProgram(){
    GLint result = 0;
    GLchar eLog[1024] = {0};
    
    // Link the shaders with Shader program
    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        LOGGER("Failed to link the program: %s\n"+ std::string(eLog));
        return;
    }
    
    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");
    
    // POINT LIGHT UNIFOMRS
    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
    
    for(size_t i=0; i<MAX_POINT_LIGHTS; i++){
        char locBuff[100] = {'\0'};
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }
    
    
    // SPOT LIGHT UNIFORMS
    uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");
    
    for(size_t i=0; i<MAX_SPOT_LIGHTS; i++){
        char locBuff[100] = {'\0'};
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderId, locBuff);
    }
    
    uniformTexture = glGetUniformLocation(shaderId, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightTransform");
    uniformDirectionShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");
    
    uniformOmniLightPos = glGetUniformLocation(shaderId, "lightPos");
    uniformFarPlane = glGetUniformLocation(shaderId, "farPlane");
    
    for(size_t i=0; i<6; i++){
        char locBuff[100] = {'\0'};
        
        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderId, locBuff);
    }
    
    for(size_t i=0; i<MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++){
        char locBuff[100] = {'\0'};
        
        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderId, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderId, locBuff);
    }
    
    // Font related
    uniformTextColor = glGetUniformLocation(shaderId, "textColor");
}

void Shader::addShader(GLuint shaderProgram, const char *shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    // attach source code to shader
    glShaderSource(theShader, 1, theCode, codeLength);
    
    // Compile
    glCompileShader(theShader);
    GLint result = 0;
    GLchar eLog[1024] = {0};
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        LOGGER("Failed to Compile the shader: "+ std::string(eLog));
        return;
    }
    
    glAttachShader(shaderProgram, theShader);
}

void Shader::useShader(){
    glUseProgram(shaderId);
}

void Shader::clearShader(){
    if(shaderId != 0){
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
    
    uniformModel = 0;
    uniformProjection = 0;
}

GLuint Shader::getModelLocation(){
    return uniformModel;
}

GLuint Shader::getProjectionLocation(){
    return uniformProjection;
}

GLuint Shader::getViewLocation(){
    return uniformView;
}

GLuint Shader::getTextColorLocation(){
    return uniformTextColor;
}

Shader::~Shader(){
    clearShader();
}

void Shader::createFromFiles(const char *vertexLocation, const char *fragmentLocation){
    LOGGER("Creating shaders from "+std::string(vertexLocation)+", "+std::string(fragmentLocation));
    std::string vertexString = readFile(vertexLocation);
    std::string fragmentString = readFile(fragmentLocation);
    
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    
    compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char *fileLocation){
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);
    
    if(!fileStream.is_open()){
        LOGGER("Failed to read file "+std::string(fileLocation)+"! File doesn't exists.");
        return "";
    }
    
    std::string line = "";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line+"\n");
    }
    
    fileStream.close();
    
    return content;
}

GLuint Shader::getAmbientColorLocation(){
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getAmbientIntensityLocation(){
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDirectionLocation(){
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getDiffuseIntensityLocation(){
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getSpecularIntensityLocation(){
    return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation(){
    return uniformShininess;
}

GLuint Shader::getEyePositionLocation(){
    return uniformEyePosition;
}

void Shader::setDirectionalLight(DirectionalLight *dLight){
    dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLight(PointLight *pLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset){
    if(lightCount > MAX_POINT_LIGHTS){
        lightCount = MAX_POINT_LIGHTS;
    }
    
    glUniform1i(uniformPointLightCount, lightCount);
    
    for(size_t i=0; i<lightCount; i++){
        pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
        
        pLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].getFarPlane());
    }
}

void Shader::setSpotLight(SpotLight *sLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset){
    if(lightCount > MAX_SPOT_LIGHTS){
        lightCount = MAX_SPOT_LIGHTS;
    }
    
    glUniform1i(uniformSpotLightCount, lightCount);
    
    for(size_t i=0; i<lightCount; i++){
        sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,
                           uniformSpotLight[i].uniformDirection,uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
        
        sLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].getFarPlane());
    }
}

void Shader::setTexture(GLuint textureUnit){
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit){
    glUniform1i(uniformDirectionShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4 *lTransform){
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

GLuint Shader::getOmniLightPosLocation(){
    return uniformOmniLightPos;
}

GLuint Shader::getFarPlaneLocation(){
    return uniformFarPlane;
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices){
    for(size_t i=0; i<6; i++){
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}

void Shader::createFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation){
    LOGGER("Creating shaders from "+std::string(vertexLocation)+", "+std::string(geometryLocation)+", "+std::string(fragmentLocation));

    std::string vertexString = readFile(vertexLocation);
    std::string geometryString = readFile(geometryLocation);
    std::string fragmentString = readFile(fragmentLocation);
    
    const char* vertexCode = vertexString.c_str();
    const char* geometryCode = geometryString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    
    compileShader(vertexCode, geometryCode, fragmentCode);
}
