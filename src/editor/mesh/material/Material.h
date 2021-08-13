//
// Created by keega on 8/9/2021.
//

#ifndef FORGE_MATERIAL_H
#define FORGE_MATERIAL_H


#include <glm/vec3.hpp>

class Material {
public:
    glm::vec3 ambient = glm::vec3(0.25f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(0.5f);
    float shininess = 32.0f;
};


#endif //FORGE_MATERIAL_H
