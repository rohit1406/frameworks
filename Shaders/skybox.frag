#version 330
                                                
in vec3 outTexCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main(){
    FragColor = texture(skybox, outTexCoords);
}
