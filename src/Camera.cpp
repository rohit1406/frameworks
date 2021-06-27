//
//  Camera.cpp
//  OpenGL
//
//  Created by Apple on 31/05/21.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed){
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;
    
    update();
}

void Camera::update(){
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool *keys, GLfloat deltaTime){
    GLfloat velocity = moveSpeed * deltaTime;
    
    if(keys[GLFW_KEY_W]){
        position += front * velocity;
    }
    
    if(keys[GLFW_KEY_S]){
        position -= front * velocity;
    }
    
    if(keys[GLFW_KEY_A]){
        position -= right * velocity;
    }
    
    if(keys[GLFW_KEY_D]){
        position += right * velocity;
    }
}


glm::mat4 Camera::calculateViewMatrix(){
    // For a third person camera, don't add position to front below
    return glm::lookAt(position, position + front, up);
}

Camera::Camera(){
    
}

Camera::~Camera(){
    
}


void Camera::mouseControl(GLfloat xChange, GLfloat yChange){
    xChange *= turnSpeed;
    yChange *= turnSpeed;
    
    yaw += xChange;
    pitch += yChange;
    
    if(pitch > 89.0f){
        pitch = 89.0f;
    }
    
    if(pitch < -89.0f){
        pitch = -89.0f;
    }
    
    update();
}

glm::vec3 Camera::getCameraPosition(){
    return position;
}

glm::vec3 Camera::getCameraDirection(){
    return glm::normalize(front);
}
