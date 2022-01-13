//
// Created by keega on 1/11/2022.
//

#ifndef FORGE_TRANSFORM_H
#define FORGE_TRANSFORM_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "glm/ext/matrix_transform.hpp"

class Transform {
public:
    //Local space information
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 Transform::getLocalModelMatrix();
};


#endif //FORGE_TRANSFORM_H
