//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_DIRECTIONALLIGHT_H
#define FORGE_DIRECTIONALLIGHT_H


#include "../Light.h"

class DirectionalLight : public Light {
public:
    glm::vec3 direction;
};


#endif //FORGE_DIRECTIONALLIGHT_H
