//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_FRAMEBUFFER_H
#define FORGE_FRAMEBUFFER_H

#include <glad/glad.h>
#include <iostream>

class Framebuffer {
public:
    unsigned int framebuffer, textureColorbuffer, depthBuffer, depthMapTexture;
    Framebuffer();

};


#endif //FORGE_FRAMEBUFFER_H
