//
// Created by keega on 6/9/2021.
//

#ifndef FORGE_VERTEX_H
#define FORGE_VERTEX_H

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};



#endif //FORGE_VERTEX_H
