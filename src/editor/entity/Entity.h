//
// Created by keega on 7/27/2021.
//

#ifndef FORGE_ENTITY_H
#define FORGE_ENTITY_H

#include <string>
#include <glm/glm.hpp>
#include "EntityType.h"


// interface for all of the objects that can be added to the editor
class Entity {

    virtual EntityType getType() = 0;
    virtual std::string getName() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual glm::mat4 getModelMatrix() = 0;
    virtual glm::vec3 getScale() = 0;
    virtual glm::vec3 getRotation() = 0;
    virtual glm::vec4 getColor() = 0;
    virtual bool getSelected() = 0;
    virtual void setSelected() = 0;

};


#endif //FORGE_ENTITY_H
