//
//  Shader.hpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "CommonValues.h"
#include "SpotLight.hpp"
#include "logger.h"

class Shader{
public:
    Shader();
    
    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
    void createFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
    
    void validate(GLuint VAO);
    
    std::string readFile(const char* fileLocation);
    
    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getEyePositionLocation();
    GLuint getOmniLightPosLocation();
    GLuint getFarPlaneLocation();
    GLuint getTextColorLocation();
    
    void setDirectionalLight(DirectionalLight *dLight);
    void setPointLight(PointLight *pLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset);
    void setSpotLight(SpotLight *sLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset);
    void setTexture(GLuint textureUnit);
    void setDirectionalShadowMap(GLuint textureUnit);
    void setDirectionalLightTransform(glm::mat4 *lTransform);
    void setLightMatrices(std::vector<glm::mat4> lightMatrices);
    
    void useShader();
    void clearShader();
    
    ~Shader();
private:
    int pointLightCount;
    int spotLightCount;
    
    GLuint shaderId, uniformProjection, uniformModel, uniformView;
    GLuint uniformEyePosition, uniformSpecularIntensity, uniformShininess;
    GLuint uniformTexture;
    GLuint uniformDirectionalLightTransform, uniformDirectionShadowMap;
    GLuint uniformOmniLightPos, uniformFarPlane;
    
    GLuint uniformLightMatrices[6];
    
    struct{
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;
    }uniformDirectionalLight;
    
    GLuint uniformPointLightCount;
    struct{
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    }uniformPointLight[MAX_POINT_LIGHTS];
    
    GLuint uniformSpotLightCount;
    struct{
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
        
        GLuint uniformDirection;
        GLuint uniformEdge;
    }uniformSpotLight[MAX_SPOT_LIGHTS];
    
    struct{
        GLuint shadowMap;
        GLuint farPlane;
    }uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
    
    // Font related uniforms
    GLuint uniformTextColor;
    
    void compileShader(const char* vertexCode, const char* fragmentCode);
    void compileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
    void addShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);
    void compileProgram();
};
#endif /* Shader_hpp */
