//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_POINTLIGHT_H
#define FORGE_POINTLIGHT_H

#include "../Light.h"

class PointLight : public Light {
    glm::vec3 position;

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};


#endif //FORGE_POINTLIGHT_H
