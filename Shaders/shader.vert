#version 330
                                                
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
                                                
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 directionalLightTransform;
                                                
out vec4 vCol;
out vec2 texCoords;
out vec3 outNorm;
out vec3 fragPos;
out vec4 directionalLightSpacePos;
                                                
void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    directionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0f);
    
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
    texCoords = tex;
    
    // converting to mat3: normal is just a direction so no need to transform normals with model matrix.
    // By converting to mat3 we are nullifying the fourth column of matrix which represents the location of the model so the normal won't get transalated when multiplied.
    // The other way would be to pass norm vector as vec4 with 4th value as 0.
    // Use used transpose and inverse to invert the scaling calculations on the model to account for non-uniform scaling
    // And also rotations will be present to calculate on the normal
    outNorm = mat3(transpose(inverse(model))) * norm;
    
    // Calculate position of fragment in world space
    fragPos = (model * vec4(pos, 1.0)).xyz;
}
