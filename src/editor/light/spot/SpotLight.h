//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_SPOTLIGHT_H
#define FORGE_SPOTLIGHT_H


#include "../Light.h"

class SpotLight : public Light {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};


#endif //FORGE_SPOTLIGHT_H
