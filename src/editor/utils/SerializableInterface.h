//
// Created by keega on 8/5/2021.
//

#ifndef FORGE_SERIALIZABLEINTERFACE_H
#define FORGE_SERIALIZABLEINTERFACE_H


#include <yaml-cpp/emitter.h>
#include <glm/vec3.hpp>

class SerializableInterface {
public:
    virtual void serialize(const std::string &filepath) = 0;
    virtual void deserialize() = 0;
};


#endif //FORGE_SERIALIZABLEINTERFACE_H
