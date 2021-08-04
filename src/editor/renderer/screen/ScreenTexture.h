//
// Created by keega on 8/3/2021.
//

#ifndef FORGE_SCREENTEXTURE_H
#define FORGE_SCREENTEXTURE_H
#include <glad/glad.h>

class ScreenTexture {
public:
    unsigned int quadVAO, quadVBO;
    float quadVertices[6*4] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    ScreenTexture();
};


#endif //FORGE_SCREENTEXTURE_H
