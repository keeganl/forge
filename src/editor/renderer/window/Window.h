//
// Created by keega on 7/25/2021.
//

#ifndef FORGE_WINDOW_H
#define FORGE_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

class Window {
public:
    Window();
    GLFWwindow* getWindow();




private:
    GLFWwindow* window;
};

//static void framebuffer_size_callback(GLFWwindow* window, int width, int height);




#endif //FORGE_WINDOW_H
