#version 330
                                                
out vec4 color;
in vec4 vCol;
in vec2 texCoords;
in vec3 outNorm;
in vec3 fragPos;        // Position of fragment in world space
in vec4 directionalLightSpacePos;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight{
    Light base;
    vec3 direction;
};

struct PointLight{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight{
    PointLight base;
    vec3 direction;
    float edge;
};

struct OmniShadowMap{
    samplerCube shadowMap;
    float farPlane;
};

struct Material{
    float specularIntensity;
    float shininess;
};
uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

// Eye position - need for specular lighting calculation
uniform vec3 eyePosition;

// for omnidirectional shadow acne problem
vec3 samplerOffsetDirections[20] = vec3[](
    vec3(1,1,1), vec3(1,-1,1), vec3(-1,-1,1), vec3(-1,1,1),
    vec3(1,1,-1), vec3(1,-1,-1), vec3(-1,-1,-1), vec3(-1,1,-1),
    vec3(1,1,0), vec3(1,-1,0), vec3(-1,-1,0), vec3(-1,1,0),
    vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
    vec3(0,1,1), vec3(0,-1,1), vec3(0,-1,-1), vec3(0,1,-1)
);

float calculateDirectionalShadowFactor(DirectionalLight light){
    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
    
    // scale value between 0 and 1
    projCoords = (projCoords * 0.5) + 0.5;
    
    // requires only with Ortho view and not the projection view
    //float closestDepth = texture(directionalShadowMap, projCoords.xy).r; // Where it is
    float currentDepth = projCoords.z;                                   // How far it is
    
    // fix for shadow acne
    vec3 normal = normalize(outNorm);
    vec3 lightDir = normalize(light.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
    
    // PCF
    float shadow = 0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for(int x=-1; x<=1;x++){
        for(int y=-1; y<=1;y++){
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow +=(currentDepth - bias) > pcfDepth ? 1.0f : 0.0f;
        }
    }
    
    // float shadow = (currentDepth - bias) > closestDepth ? 1.0f : 0.0f;
    shadow /= 9.0;
    if(projCoords.z > 1){
        shadow = 0.0f;
    }
    
    return shadow;
}

float calculateOmniShadowFactor(PointLight light, int shadowIndex){
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    
    // PCF: Calculation heavy
    /*float shadow = 0.0f;
    float bias = 0.05f;
    float samples = 4.0f;
    float offset = 0.1f;
    
    for(float x=-offset; x<=offset; x+=offset / (samples * 0.5)){
        for(float y=-offset; y<=offset; y+=offset / (samples * 0.5)){
            for(float z=-offset; z<=offset; z+=offset / (samples * 0.5)){
                float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= omniShadowMaps[shadowIndex].farPlane;        // we divide distance by farPlane in fragment shader; reverting it here
                if((currentDepth - bias) > closestDepth){
                    shadow += 1.0f;
                }
            }
        }
    }
     shadow /= (samples * samples * samples);
     */
    
    // PCF: Calculation Friendly
    float shadow = 0.0f;
    float bias = 0.05f;
    int samples = 20;
    float viewDistance = length(eyePosition - fragPos);
    float diskRadius = (1.0 + (viewDistance/ omniShadowMaps[shadowIndex].farPlane)) / 25.0f;
    for(int i=0; i<samples; i++){
        float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + samplerOffsetDirections[i] * diskRadius).r;
        closestDepth *= omniShadowMaps[shadowIndex].farPlane;        // we divide distance by farPlane in fragment shader; reverting it here
        if((currentDepth - bias) > closestDepth){
            shadow += 1.0f;
        }
    }
    
    shadow /= float(samples);
    return shadow;
}
                 
vec4 calculateLightByDirection(Light light, vec3 direction, float shadowFactor){
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    
    // normalizing to make sure the dot product effectively calculate the cosine of angle
    // normalize won't affect the direction of the vector but it converts to unit vector which will make |A| = |B| = 1
    // dot product = |A| * |B| * cos(theta)
    // cos(0) = 1 - perpendicular
    // cos(90) = 0 - parallel
    float diffuseFactor = max(dot(normalize(outNorm), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
    
    vec4 specularColor = vec4(0, 0, 0, 0);
    if(diffuseFactor > 0){
        // Only direction is necessary so normalizing
        vec3 fragToEye = normalize(eyePosition - fragPos);
        
        // find reflection vector
        vec3 reflectedVertex = normalize(reflect(direction, normalize(outNorm)));
        
        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0){
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }
    
    return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

vec4 calculateDirectionLight(){
    float shadowFactor =calculateDirectionalShadowFactor(directionalLight);
    return calculateLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calculatePointLight(PointLight pLight, int shadowIndex){
    vec3 direction = fragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);
    
    float shadowFactor = calculateOmniShadowFactor(pLight, shadowIndex);
    
    vec4 color = calculateLightByDirection(pLight.base, direction, shadowFactor);
    
    // ax2 + bx + c
    float attenuation = pLight.exponent * distance * distance +
                        pLight.linear * distance +
                        pLight.constant;
    
    return (color / attenuation);
}

vec4 calculateSpotLight(SpotLight sLight, int shadowIndex){
    vec3 rayDirection = normalize(fragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);
    
    if(slFactor > sLight.edge){
        vec4 color = calculatePointLight(sLight.base, shadowIndex);
        return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
    }else{
        return vec4(0, 0, 0, 0);
    }
}

vec4 calculateSpotLights(){
    vec4 totalColor = vec4(0, 0, 0, 0);
    
    for(int i=0; i<spotLightCount; i++){
        totalColor += calculateSpotLight(spotLights[i], i + pointLightCount);
    }
    
    return totalColor;
}

vec4 calculatePointLights(){
    vec4 totalColor = vec4(0, 0, 0, 0);
    
    for(int i=0; i<pointLightCount; i++){
        totalColor += calculatePointLight(pointLights[i], i);
    }
    
    return totalColor;
}
void main(){
    vec4 finalColor =calculateDirectionLight();
    finalColor += calculatePointLights();
    finalColor += calculateSpotLights();
    color = texture(theTexture, texCoords) * finalColor;
}
