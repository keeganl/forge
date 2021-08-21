//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_DIRECTIONALLIGHT_H
#define FORGE_DIRECTIONALLIGHT_H


#include "../Light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(std::string const &path,
          bool gamma = false,
          // FIXME: feels hacky
          std::string const &type = "directional_light",
          glm::vec4 const &objColor = glm::vec4(1.0, 1.0, 1.0, 1.0),
          glm::vec3 const &pos = glm::vec3(0.0f, 0.0f, 0.0f),
          glm::mat4 const &matrix = glm::mat4(1.0f),
          glm::vec3 const &rotationVals =  glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3 const &scales = glm::vec3(0.0f, 0.0f, 0.0f),
          float const &scale =1.0f) :
          Light(path, gamma, type, objColor, pos, matrix, rotationVals, scales, scale) {}
};


#endif //FORGE_DIRECTIONALLIGHT_H
