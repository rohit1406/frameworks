//
//  main.cpp
//  OpenGL
//
//  Created by Apple on 29/05/21.
//
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"



CustomWindow *window;
Camera camera;
Renderer renderer;

// Radians ranges between 0 and 2Pi and not 0 and 360 in which degrees does
// const float toRadians = 3.14159265f / 180.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastTIme = 0.0f;


int main(int argc, const char * argv[]) {
    window = new CustomWindow(800, 600);  // 1366 * 768, 1280 * 1024, 1024 * 768
    
    window->intialize();
    renderer = Renderer(window);
    renderer.init();
    
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f,
                    5.0f,   // Sensitivity of movement
                    0.1f    // Sensitivity of mouse control
                    );
    
    
    // Loop until window closed
    while(!window->getShouldClose()){
        GLfloat now = glfwGetTime();        // SDL_GetPerformanceCounter()
        deltaTime = now - lastTIme;         // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency()
        lastTIme = now;
        
        // Get + Handle user input events
        glfwPollEvents();
        
        camera.keyControl(window->getKeys(), deltaTime);
        camera.mouseControl(window->getXChange(), window->getYChange());
        
        renderer.render(camera);
        
        window->swapBuffers();
    }
    return 0;
}
