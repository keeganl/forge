//
// Created by keega on 1/11/2022.
//

#include "Transform.h"

glm::mat4 Transform::getLocalModelMatrix() {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(eulerRotation.x),
                                             glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(eulerRotation.y),
                                             glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(eulerRotation.z),
                                             glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), pos) *
    roationMatrix *
            glm::scale(glm::mat4(1.0f), scale);
    }