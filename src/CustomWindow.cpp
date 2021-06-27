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
    
    LOGGER("Creating window of size "+std::to_string(width)+"*"+std::to_string(height));
}

CustomWindow::CustomWindow(GLint windowWidth, GLint windowHeight){
    width = windowWidth;
    height = windowHeight;
    xChange = 0.0f;
    yChange = 0.0f;
    
    for(size_t i=0; i<1024; i++){
        keys[i] = 0;
    }
    
    LOGGER("Creating window of size "+std::to_string(width)+"*"+std::to_string(height));
}

int CustomWindow::intialize(){
    //Initialize GLFW
    if(!glfwInit()){
        LOGGER("Failed to initialise GLFW! Terminating GLFW.");
        glfwTerminate();
        return 1;
    }
    LOGGER("GLFW initialised.");
    
    // Setup GLFW window properties
    // OpenGL Version: 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - No Backward Compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    LOGGER("OpenGL properties set with OpenGL version 3.3");
    
    mainWindow = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if(!mainWindow){
        LOGGER("Failed to create GLFW Window! Terminating GLFW.");
        glfwTerminate();
        return 1;
    }
    LOGGER("Main Window Created.");
    
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
        LOGGER("Failed to initialise GLEW!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    LOGGER("GLEW Initialised.");
    
    // Enable OpenGL features
    glEnable(GL_DEPTH_TEST);
    LOGGER("Depth test enabled.");
    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    LOGGER("Viewport set to "+std::to_string(bufferWidth)+"*"+std::to_string(bufferHeight));
    glfwSetWindowUserPointer(mainWindow, this);
    LOGGER("Window Initialisation Completed.");
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
