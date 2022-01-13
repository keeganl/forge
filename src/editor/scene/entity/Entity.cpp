//
// Created by keega on 1/12/2022.
//

#include "Entity.h"

Entity::Entity() {}

template<typename... TArgs>
void Entity::addChild(const TArgs&... args)
{
    children.emplace_back(std::move(std::make_unique<Entity>(args...)));
    children.back()->parent = reinterpret_cast<std::unique_ptr<Entity> *>(this);
}


[[maybe_unused]] void Entity::updateSelfAndChild()
{
    if (parent)
        transform.modelMatrix = parent->get()->transform.modelMatrix * transform.getLocalModelMatrix();
    else
        transform.modelMatrix = transform.getLocalModelMatrix();

    for (auto& child : children)
    {
        child->updateSelfAndChild();
    }
}