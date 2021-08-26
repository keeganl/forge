//
// Created by keega on 6/22/2021.
//

#ifndef FORGE_LIGHT_H
#define FORGE_LIGHT_H
#include <glad/glad.h> // holds all OpenGL type declarations

#include <vector>
#include "../mesh/Model.h"
#include "../utils/shader-manager/Shader.h"


// Light needs to be abstracted from model
class Light : public Model {
public:
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    float innerCutoff = glm::cos(glm::radians(12.5f));
    float outerCutoff = glm::cos(glm::radians(15.0f));


    Light(std::string const &path,
          bool gamma = false,
          std::string const &type = "light",
          glm::vec4 const &objColor = glm::vec4(1.0, 1.0, 1.0, 1.0),
          glm::vec3 const &pos = glm::vec3(0.0f, 0.0f, 0.0f),
          glm::mat4 const &matrix = glm::mat4(1.0f),
          glm::vec3 const &rotationVals =  glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3 const &scales = glm::vec3(0.0f, 0.0f, 0.0f),
          float const &scale =1.0f) :
          Model(path, gamma, type, objColor, pos, matrix, rotationVals, scales, scale) {}

};


#endif //FORGE_LIGHT_H
