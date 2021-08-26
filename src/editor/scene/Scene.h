//
// Created by keega on 8/14/2021.
//

#ifndef FORGE_SCENE_H
#define FORGE_SCENE_H


#include <memory>
#include <vector>
#include "../mesh/Model.h"
#include "../light/Light.h"
#include "../camera/Camera.h"

class Scene {
public:
    std::vector<std::shared_ptr<Model>> models;
    std::vector<std::shared_ptr<Light>> lights;
    Camera camera;

    Scene();
};


#endif //FORGE_SCENE_H
