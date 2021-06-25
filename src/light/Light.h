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

    Light(std::string const &path, bool gamma = false, std::string const &type = "light", glm::vec4 const &objColor = glm::vec4(1.0, 1.0, 1.0, 1.0)) : Model(path, gamma, type, objColor) {}

};


#endif //FORGE_LIGHT_H
