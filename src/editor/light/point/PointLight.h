//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_POINTLIGHT_H
#define FORGE_POINTLIGHT_H

#include "../Light.h"

class PointLight : public Light {
public:
    PointLight(std::string const &path,
                     bool gamma = false,
                     std::string const &type = "point_light",
                     glm::vec4 const &objColor = glm::vec4(1.0, 1.0, 1.0, 1.0),
                     glm::vec3 const &pos = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::mat4 const &matrix = glm::mat4(1.0f),
                     glm::vec3 const &rotationVals =  glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 const &scales = glm::vec3(0.0f, 0.0f, 0.0f),
                     float const &scale =1.0f) :
                     Light(path, gamma, type, objColor, pos, matrix, rotationVals, scales, scale) {}
};


#endif //FORGE_POINTLIGHT_H
