//
//  CustomWindow.hpp
//  OpenGL
//
//  Created by Apple on 30/05/21.
//

#ifndef CustomWindow_hpp
#define CustomWindow_hpp

#include <stdio.h>

#include <GL/glew.h>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "logger.h"

class CustomWindow{
public:
    CustomWindow();
    CustomWindow(GLint width, GLint height);
    int intialize();
    
    GLfloat getBufferWidth(){
        return bufferWidth;
    }
    
    GLfloat getBufferHeight(){
        return bufferHeight;
    }
    
    bool getShouldClose(){
        return glfwWindowShouldClose(mainWindow);
    }
    
    void swapBuffers(){
        glfwSwapBuffers(mainWindow);
    }
    
    bool* getKeys(){
        return keys;
    }
    
    GLfloat getXChange();
    GLfloat getYChange();
    
    ~CustomWindow();
    
private:
    GLFWwindow *mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    
    bool keys[1024];
    
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;
    
    void createCallbacks();
    static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};
#endif /* CustomWindow_hpp */
