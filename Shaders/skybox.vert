#version 330
                                                
layout(location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;

out vec3 outTexCoords;

void main(){
    outTexCoords = pos;
    gl_Position = projection * view * vec4(pos, 1.0f);
}
