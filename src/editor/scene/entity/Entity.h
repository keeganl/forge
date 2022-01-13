//
// Created by keega on 1/12/2022.
//

#ifndef FORGE_ENTITY_H
#define FORGE_ENTITY_H

#include "./transform/Transform.h"
#include "../../mesh/Model.h"
#include <vector>
#include <memory>

class Entity {
public:
    Transform transform;
    std::list<std::unique_ptr<Entity>> children;
    std::unique_ptr<Entity>* parent = nullptr;
    Model mesh;

    Entity();
    ~Entity() = default;


    template<typename... TArgs>
    void addChild(const TArgs&... args);

    [[maybe_unused]] void updateSelfAndChild();

};


#endif //FORGE_ENTITY_H
