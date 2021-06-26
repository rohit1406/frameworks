//
//  CustomWindow.cpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#include "CustomWindow.hpp"

CustomWindow::CustomWindow(){
    width = 800;
    width = 600;
    xChange = 0.0f;
    yChange = 0.0f;
    
    for(size_t i=0; i<1024; i++){
        keys[i] = 0;
    }
}

CustomWindow::CustomWindow(GLint windowWidth, GLint windowHeight){
    width = windowWidth;
    height = windowHeight;
    xChange = 0.0f;
    yChange = 0.0f;
    
    for(size_t i=0; i<1024; i++){
        keys[i] = 0;
    }
}

int CustomWindow::intialize(){
    //Initialize GLFW
    if(!glfwInit()){
        printf("Failed to initialize GLFW!");
        glfwTerminate();
        return 1;
    }
    
    // Setup GLFW window properties
    // OpenGL Version: 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - No Backward Compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    mainWindow = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if(!mainWindow){
        printf("Failed to create GLFW Window!");
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // Set context for GLFW to use
    glfwMakeContextCurrent(mainWindow);
    
    // Handle Key + Mouse input
    createCallbacks();
    
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK){
        printf("Failed to initialize GLEW!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    // Enable OpenGL features
    glEnable(GL_DEPTH_TEST);
    
    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

CustomWindow::~CustomWindow(){
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

void CustomWindow::handleKeys(GLFWwindow *window, int key, int code, int action, int mode){
    CustomWindow *theWindow = static_cast<CustomWindow*>(glfwGetWindowUserPointer(window));
    
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if(key >= 0 && key < 1024){
        if(action == GLFW_PRESS){
            theWindow->keys[key] = true;
            //printf("Pressed: %d\n", key);
        }else if(action == GLFW_RELEASE){
            theWindow->keys[key] = false;
            //printf("Release: %d\n", key);
        }
    }
}

void CustomWindow::createCallbacks(){
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void CustomWindow::handleMouse(GLFWwindow *window, double xPos, double yPos){
    CustomWindow *theWindow = static_cast<CustomWindow*>(glfwGetWindowUserPointer(window));
    
    if(theWindow->mouseFirstMoved){
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }
    
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;
    
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

GLfloat CustomWindow::getXChange(){
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat CustomWindow::getYChange(){
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}
