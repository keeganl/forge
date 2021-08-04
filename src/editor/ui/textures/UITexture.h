//
// Created by keega on 8/1/2021.
//

#ifndef FORGE_UITEXTURE_H
#define FORGE_UITEXTURE_H


#include <glad/glad.h>
#include <string>

class UITexture {
public:
    std::string filepath;
    int width = 0;
    int height = 0;
    GLuint textureID = 0;

    UITexture(const char* filename);
};


#endif //FORGE_UITEXTURE_H
