//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_WINDOW_H
#define FORGE_WINDOW_H

#include "../../../external/stb/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Window {
public:
    GLFWwindow* windowInstance;
    GLFWimage windowIcon;

    Window();
    void swapAndPoll();
    void destroyWindow();
};


#endif //FORGE_WINDOW_H
