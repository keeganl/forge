//
// Created by keega on 6/22/2021.
//

#ifndef FORGE_LIGHT_H
#define FORGE_LIGHT_H
#include <glad/glad.h> // holds all OpenGL type declarations

#include <vector>
#include "../mesh/Model.h"
#include "../utils/shader-manager/Shader.h"


class Light : public Model {
public:
    float diffuse;
    float ambient;

};


#endif //FORGE_LIGHT_H
