#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main(){
    float fragDistance = length(FragPos.xyz - lightPos);
    fragDistance = fragDistance / farPlane;
    gl_FragDepth = fragDistance;
}
